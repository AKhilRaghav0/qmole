/*
 * pattern.c: Implemetation of the template match compilation and lookup
 *
 * Reference:
 *   http://www.w3.org/TR/1999/REC-xslt-19991116
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

/*
 * TODO: handle pathological cases like *[*[@a="b"]]
 * TODO: detect [number] at compilation, optimize accordingly
 */

#define IN_LIBXSLT
#include "libxslt.h"

#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/valid.h>
#include <libxml/hash.h>
#include <libxml/xmlerror.h>
#include <libxml/parserInternals.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "imports.h"
#include "templates.h"
#include "keys.h"
#include "pattern.h"
#include "documents.h"

#ifdef WITH_XSLT_DEBUG
#define WITH_XSLT_DEBUG_PATTERN
#endif

/*
 * Types are private:
 */

typedef enum {
    XSLT_OP_END=0,
    XSLT_OP_ROOT,
    XSLT_OP_ELEM,
    XSLT_OP_CHILD,
    XSLT_OP_ATTR,
    XSLT_OP_PARENT,
    XSLT_OP_ANCESTOR,
    XSLT_OP_ID,
    XSLT_OP_KEY,
    XSLT_OP_NS,
    XSLT_OP_ALL,
    XSLT_OP_PI,
    XSLT_OP_COMMENT,
    XSLT_OP_TEXT,
    XSLT_OP_NODE,
    XSLT_OP_PREDICATE
} xsltOp;

typedef struct _xsltStepState xsltStepState;
typedef xsltStepState *xsltStepStatePtr;
struct _xsltStepState {
    int step;
    xmlNodePtr node;
};

typedef struct _xsltStepStates xsltStepStates;
typedef xsltStepStates *xsltStepStatesPtr;
struct _xsltStepStates {
    int nbstates;
    int maxstates;
    xsltStepStatePtr states;
};

typedef struct _xsltStepOp xsltStepOp;
typedef xsltStepOp *xsltStepOpPtr;
struct _xsltStepOp {
    xsltOp op;
    xmlChar *value;
    xmlChar *value2;
    xmlChar *value3;
    xmlXPathCompExprPtr comp;
    /*
     * Optimisations for count
     */
    int        previousExtra;
    int        indexExtra;
    int        lenExtra;
};

struct _xsltCompMatch {
    struct _xsltCompMatch *next; /* siblings in the name hash */
    float priority;              /* the priority */
    const xmlChar *pattern;       /* the pattern */
    const xmlChar *mode;         /* the mode */
    const xmlChar *modeURI;      /* the mode URI */
    xsltTemplatePtr template;    /* the associated template */

    int direct;
    /* TODO fix the statically allocated size steps[] */
    int nbStep;
    int maxStep;
    xmlNsPtr *nsList;		/* the namespaces in scope */
    int nsNr;			/* the number of namespaces in scope */
    xsltStepOp steps[40];        /* ops for computation */
};

typedef struct _xsltParserContext xsltParserContext;
typedef xsltParserContext *xsltParserContextPtr;
struct _xsltParserContext {
    xsltStylesheetPtr style;		/* the stylesheet */
    xsltTransformContextPtr ctxt;	/* the transformation or NULL */
    const xmlChar *cur;			/* the current char being parsed */
    const xmlChar *base;		/* the full expression */
    xmlDocPtr      doc;			/* the source document */
    xmlNodePtr    elem;			/* the source element */
    int error;				/* error code */
    xsltCompMatchPtr comp;		/* the result */
};

/************************************************************************
 * 									*
 * 			Type functions 					*
 * 									*
 ************************************************************************/

/**
 * xsltNewCompMatch:
 *
 * Create a new XSLT CompMatch
 *
 * Returns the newly allocated xsltCompMatchPtr or NULL in case of error
 */
static xsltCompMatchPtr
xsltNewCompMatch(void) {
    xsltCompMatchPtr cur;

    cur = (xsltCompMatchPtr) xmlMalloc(sizeof(xsltCompMatch));
    if (cur == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltNewCompMatch : out of memory error\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltCompMatch));
    cur->maxStep = 40;
    cur->nsNr = 0;
    cur->nsList = NULL;
    cur->direct = 0;
    return(cur);
}

/**
 * xsltFreeCompMatch:
 * @comp:  an XSLT comp
 *
 * Free up the memory allocated by @comp
 */
static void
xsltFreeCompMatch(xsltCompMatchPtr comp) {
    xsltStepOpPtr op;
    int i;

    if (comp == NULL)
	return;
    if (comp->pattern != NULL)
	xmlFree((xmlChar *)comp->pattern);
    if (comp->nsList != NULL)
	xmlFree(comp->nsList);
    for (i = 0;i < comp->nbStep;i++) {
	op = &comp->steps[i];
	if (op->value != NULL)
	    xmlFree(op->value);
	if (op->value2 != NULL)
	    xmlFree(op->value2);
	if (op->value3 != NULL)
	    xmlFree(op->value3);
	if (op->comp != NULL)
	    xmlXPathFreeCompExpr(op->comp);
    }
    memset(comp, -1, sizeof(xsltCompMatch));
    xmlFree(comp);
}

/**
 * xsltFreeCompMatchList:
 * @comp:  an XSLT comp list
 *
 * Free up the memory allocated by all the elements of @comp
 */
void
xsltFreeCompMatchList(xsltCompMatchPtr comp) {
    xsltCompMatchPtr cur;

    while (comp != NULL) {
	cur = comp;
	comp = comp->next;
	xsltFreeCompMatch(cur);
    }
}

/**
 * xsltNormalizeCompSteps:
 * @payload: pointer to template hash table entry
 * @data: pointer to the stylesheet
 * @name: template match name
 *
 * This is a hashtable scanner function to normalize the compiled
 * steps of an imported stylesheet.
 */
void xsltNormalizeCompSteps(void *payload,
        void *data, const xmlChar *name ATTRIBUTE_UNUSED) {
    xsltCompMatchPtr comp = payload;
    xsltStylesheetPtr style = data;
    int ix;

    for (ix = 0; ix < comp->nbStep; ix++) {
        comp->steps[ix].previousExtra += style->extrasNr;
        comp->steps[ix].indexExtra += style->extrasNr;
        comp->steps[ix].lenExtra += style->extrasNr;
    }
}

/**
 * xsltNewParserContext:
 * @style:  the stylesheet
 * @ctxt:  the transformation context, if done at run-time
 *
 * Create a new XSLT ParserContext
 *
 * Returns the newly allocated xsltParserContextPtr or NULL in case of error
 */
static xsltParserContextPtr
xsltNewParserContext(xsltStylesheetPtr style, xsltTransformContextPtr ctxt) {
    xsltParserContextPtr cur;

    cur = (xsltParserContextPtr) xmlMalloc(sizeof(xsltParserContext));
    if (cur == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltNewParserContext : malloc failed\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltParserContext));
    cur->style = style;
    cur->ctxt = ctxt;
    return(cur);
}

/**
 * xsltFreeParserContext:
 * @ctxt:  an XSLT parser context
 *
 * Free up the memory allocated by @ctxt
 */
static void
xsltFreeParserContext(xsltParserContextPtr ctxt) {
    if (ctxt == NULL)
	return;
    memset(ctxt, -1, sizeof(xsltParserContext));
    xmlFree(ctxt);
}

/**
 * xsltCompMatchAdd:
 * @comp:  the compiled match expression
 * @op:  an op
 * @value:  the first value
 * @value2:  the second value
 * @novar:  flag to set XML_XPATH_NOVAR
 *
 * Add an step to an XSLT Compiled Match
 *
 * Returns -1 in case of failure, 0 otherwise.
 */
static int
xsltCompMatchAdd(xsltParserContextPtr ctxt, xsltCompMatchPtr comp,
                 xsltOp op, xmlChar * value, xmlChar * value2, int novar)
{
    if (comp->nbStep >= 40) {
        xsltTransformError(NULL, NULL, NULL,
                         "xsltCompMatchAdd: overflow\n");
        return (-1);
    }
    comp->steps[comp->nbStep].op = op;
    comp->steps[comp->nbStep].value = value;
    comp->steps[comp->nbStep].value2 = value2;
    if (ctxt->ctxt != NULL) {
	comp->steps[comp->nbStep].previousExtra =
	    xsltAllocateExtraCtxt(ctxt->ctxt);
	comp->steps[comp->nbStep].indexExtra =
	    xsltAllocateExtraCtxt(ctxt->ctxt);
	comp->steps[comp->nbStep].lenExtra =
	    xsltAllocateExtraCtxt(ctxt->ctxt);
    } else {
	comp->steps[comp->nbStep].previousExtra =
	    xsltAllocateExtra(ctxt->style);
	comp->steps[comp->nbStep].indexExtra =
	    xsltAllocateExtra(ctxt->style);
	comp->steps[comp->nbStep].lenExtra =
	    xsltAllocateExtra(ctxt->style);
    }
    if (op == XSLT_OP_PREDICATE) {
	xmlXPathContextPtr xctxt;

	if (ctxt->style != NULL)
	    xctxt = xmlXPathNewContext(ctxt->style->doc);
	else
	    xctxt = xmlXPathNewContext(NULL);
#ifdef XML_XPATH_NOVAR
	if (novar != 0)
	    xctxt->flags = XML_XPATH_NOVAR;
#endif
	if (ctxt->style != NULL)
	    xctxt->dict = ctxt->style->dict;
	comp->steps[comp->nbStep].comp = xmlXPathCtxtCompile(xctxt, value);
	xmlXPathFreeContext(xctxt);
	if (comp->steps[comp->nbStep].comp == NULL) {
	    xsltTransformError(NULL, ctxt->style, ctxt->elem,
		    "Failed to compile predicate\n");
	    if (ctxt->style != NULL)
		ctxt->style->errors++;
	}
    }
    comp->nbStep++;
    return (0);
}

/**
 * xsltSwapTopCompMatch:
 * @comp:  the compiled match expression
 *
 * reverse the two top steps.
 */
static void
xsltSwapTopCompMatch(xsltCompMatchPtr comp) {
    int i;
    int j = comp->nbStep - 1;

    if (j > 0) {
	register xmlChar *tmp;
	register xsltOp op;
	register xmlXPathCompExprPtr expr; 
	i = j - 1;
	tmp = comp->steps[i].value;
	comp->steps[i].value = comp->steps[j].value;
	comp->steps[j].value = tmp;
	tmp = comp->steps[i].value2;
	comp->steps[i].value2 = comp->steps[j].value2;
	comp->steps[j].value2 = tmp;
	op = comp->steps[i].op;
	comp->steps[i].op = comp->steps[j].op;
	comp->steps[j].op = op;
	expr = comp->steps[i].comp;
	comp->steps[i].comp = comp->steps[j].comp;
	comp->steps[j].comp = expr;
    }
}

/**
 * xsltReverseCompMatch:
 * @comp:  the compiled match expression
 *
 * reverse all the stack of expressions
 */
static void
xsltReverseCompMatch(xsltCompMatchPtr comp) {
    int i = 0;
    int j = comp->nbStep - 1;

    while (j > i) {
	register xmlChar *tmp;
	register xsltOp op;
	register xmlXPathCompExprPtr expr; 
	tmp = comp->steps[i].value;
	comp->steps[i].value = comp->steps[j].value;
	comp->steps[j].value = tmp;
	tmp = comp->steps[i].value2;
	comp->steps[i].value2 = comp->steps[j].value2;
	comp->steps[j].value2 = tmp;
	op = comp->steps[i].op;
	comp->steps[i].op = comp->steps[j].op;
	comp->steps[j].op = op;
	expr = comp->steps[i].comp;
	comp->steps[i].comp = comp->steps[j].comp;
	comp->steps[j].comp = expr;
	j--;
	i++;
    }
    comp->steps[comp->nbStep++].op = XSLT_OP_END;
    /*
     * detect consecutive XSLT_OP_PREDICATE indicating a direct
     * matching should be done.
     */
    for (i = 0;i < comp->nbStep - 1;i++) {
        if ((comp->steps[i].op == XSLT_OP_PREDICATE) &&
	    (comp->steps[i + 1].op == XSLT_OP_PREDICATE)) {

	    comp->direct = 1;
	    if (comp->pattern[0] != '/') {
		xmlChar *query;

		query = xmlStrdup((const xmlChar *)"//");
		query = xmlStrcat(query, comp->pattern);

		xmlFree((xmlChar *) comp->pattern);
		comp->pattern = query;
	    }
	    break;
	}
    }
}

/************************************************************************
 * 									*
 * 		The interpreter for the precompiled patterns		*
 * 									*
 ************************************************************************/

static int
xsltPatPushState(xsltStepStates *states, int step, xmlNodePtr node) {
    if ((states->states == NULL) || (states->maxstates <= 0)) {
        states->maxstates = 4;
	states->nbstates = 0;
	states->states = xmlMalloc(4 * sizeof(xsltStepState));
    }
    else if (states->maxstates <= states->nbstates) {
        xsltStepState *tmp;

	tmp = (xsltStepStatePtr) xmlRealloc(states->states,
			       2 * states->maxstates * sizeof(xsltStepState));
	if (tmp == NULL)
	    return(-1);
	states->states = tmp;
	states->maxstates *= 2;
    }
    states->states[states->nbstates].step = step;
    states->states[states->nbstates++].node = node;
#if 0
    fprintf(stderr, "Push: %d, %s\n", step, node->name);
#endif
    return(0);
}

/**
 * xsltTestCompMatchDirect:
 * @ctxt:  a XSLT process context
 * @comp: the precompiled pattern
 * @node: a node
 * @nsList: the namespaces in scope
 * @nsNr: the number of namespaces in scope
 *
 * Test whether the node matches the pattern, do a direct evalutation
 * and not a step by step evaluation.
 *
 * Returns 1 if it matches, 0 if it doesn't and -1 in case of failure
 */
static int
xsltTestCompMatchDirect(xsltTransformContextPtr ctxt, xsltCompMatchPtr comp,
	                xmlNodePtr node, xmlNsPtr *nsList, int nsNr) {
    xsltStepOpPtr sel = NULL;
    xmlDocPtr prevdoc;
    xmlDocPtr doc;
    xmlXPathObjectPtr list;
    int ix, j;
    int nocache = 0;
    int isRVT;

    doc = node->doc;
    if (XSLT_IS_RES_TREE_FRAG(doc))
	isRVT = 1;
    else
	isRVT = 0;
    sel = &comp->steps[0]; /* store extra in first step arbitrarily */

    prevdoc = (xmlDocPtr)
	XSLT_RUNTIME_EXTRA(ctxt, sel->previousExtra, ptr);
    ix = XSLT_RUNTIME_EXTRA(ctxt, sel->indexExtra, ival);
    list = (xmlXPathObjectPtr)
	XSLT_RUNTIME_EXTRA_LST(ctxt, sel->lenExtra);
    
    if ((list == NULL) || (prevdoc != doc)) {
	xmlXPathObjectPtr newlist;
	xmlNodePtr parent = node->parent;
	xmlDocPtr olddoc;
	xmlNodePtr oldnode;
	int oldNsNr;
	xmlNsPtr *oldNamespaces;

	oldnode = ctxt->xpathCtxt->node;
	olddoc = ctxt->xpathCtxt->doc;
	oldNsNr = ctxt->xpathCtxt->nsNr;
	oldNamespaces = ctxt->xpathCtxt->namespaces;
	ctxt->xpathCtxt->node = node;
	ctxt->xpathCtxt->doc = doc;
	ctxt->xpathCtxt->namespaces = nsList;
	ctxt->xpathCtxt->nsNr = nsNr;
	newlist = xmlXPathEval(comp->pattern, ctxt->xpathCtxt);
	ctxt->xpathCtxt->node = oldnode;
	ctxt->xpathCtxt->doc = olddoc;
	ctxt->xpathCtxt->namespaces = oldNamespaces;
	ctxt->xpathCtxt->nsNr = oldNsNr;
	if (newlist == NULL)
	    return(-1);
	if (newlist->type != XPATH_NODESET) {
	    xmlXPathFreeObject(newlist);
	    return(-1);
	}
	ix = 0;

	if ((parent == NULL) || (node->doc == NULL) || isRVT)
	    nocache = 1;
	
	if (nocache == 0) {
	    if (list != NULL)
		xmlXPathFreeObject(list);
	    list = newlist;

	    XSLT_RUNTIME_EXTRA_LST(ctxt, sel->lenExtra) =
		(void *) list;
	    XSLT_RUNTIME_EXTRA(ctxt, sel->previousExtra, ptr) =
		(void *) doc;
	    XSLT_RUNTIME_EXTRA(ctxt, sel->indexExtra, ival) =
		0;
	    XSLT_RUNTIME_EXTRA_FREE(ctxt, sel->lenExtra) =
		(xmlFreeFunc) xmlXPathFreeObject;
	} else
	    list = newlist;
    }
    if ((list->nodesetval == NULL) ||
	(list->nodesetval->nodeNr <= 0)) {
	if (nocache == 1)
	    xmlXPathFreeObject(list);
	return(0);
    }
    /* TODO: store the index and use it for the scan */
    if (ix == 0) {
	for (j = 0;j < list->nodesetval->nodeNr;j++) {
	    if (list->nodesetval->nodeTab[j] == node) {
		if (nocache == 1)
		    xmlXPathFreeObject(list);
		return(1);
	    }
	}
    } else {
    }
    if (nocache == 1)
	xmlXPathFreeObject(list);
    return(0);
}

/**
 * xsltTestCompMatch:
 * @ctxt:  a XSLT process context
 * @comp: the precompiled pattern
 * @node: a node
 * @mode:  the mode name or NULL
 * @modeURI:  the mode URI or NULL
 *
 * Test whether the node matches the pattern
 *
 * Returns 1 if it matches, 0 if it doesn't and -1 in case of failure
 */
static int
xsltTestCompMatch(xsltTransformContextPtr ctxt, xsltCompMatchPtr comp,
	          xmlNodePtr node, const xmlChar *mode,
		  const xmlChar *modeURI) {
    int i;
    xsltStepOpPtr step, sel = NULL;
    xsltStepStates states = {0, 0, NULL}; /* // may require backtrack */

    if ((comp == NULL) || (node == NULL) || (ctxt == NULL)) {
	xsltTransformError(ctxt, NULL, node,
		"xsltTestCompMatch: null arg\n");
        return(-1);
    }
    if (mode != NULL) {
	if (comp->mode == NULL)
	    return(0);
	/*
	 * both mode strings must be interned on the stylesheet dictionary
	 */
	if (comp->mode != mode)
	    return(0);
    } else {
	if (comp->mode != NULL)
	    return(0);
    }
    if (modeURI != NULL) {
	if (comp->modeURI == NULL)
	    return(0);
	/*
	 * both modeURI strings must be interned on the stylesheet dictionary
	 */
	if (comp->modeURI != modeURI)
	    return(0);
    } else {
	if (comp->modeURI != NULL)
	    return(0);
    }

    i = 0;
restart:
    for (;i < comp->nbStep;i++) {
	step = &comp->steps[i];
	if (step->op != XSLT_OP_PREDICATE)
	    sel = step;
	switch (step->op) {
            case XSLT_OP_END:
		goto found;
            case XSLT_OP_ROOT:
		if ((node->type == XML_DOCUMENT_NODE) ||
#ifdef LIBXML_DOCB_ENABLED
		    (node->type == XML_DOCB_DOCUMENT_NODE) ||
#endif
		    (node->type == XML_HTML_DOCUMENT_NODE))
		    continue;
		if ((node->type == XML_ELEMENT_NODE) && (node->name[0] == ' '))
		    continue;
		goto rollback;
            case XSLT_OP_ELEM:
		if (node->type != XML_ELEMENT_NODE)
		    goto rollback;
		if (step->value == NULL)
		    continue;
		if (step->value[0] != node->name[0])
		    goto rollback;
		if (!xmlStrEqual(step->value, node->name))
		    goto rollback;

		/* Namespace test */
		if (node->ns == NULL) {
		    if (step->value2 != NULL)
			goto rollback;
		} else if (node->ns->href != NULL) {
		    if (step->value2 == NULL)
			goto rollback;
		    if (!xmlStrEqual(step->value2, node->ns->href))
			goto rollback;
		}
		continue;
            case XSLT_OP_CHILD: {
		xmlNodePtr lst;

		if ((node->type != XML_ELEMENT_NODE) &&
		    (node->type != XML_DOCUMENT_NODE) &&
#ifdef LIBXML_DOCB_ENABLED
		    (node->type != XML_DOCB_DOCUMENT_NODE) &&
#endif
		    (node->type != XML_HTML_DOCUMENT_NODE))
		    goto rollback;

		lst = node->children;

		if (step->value != NULL) {
		    while (lst != NULL) {
			if ((lst->type == XML_ELEMENT_NODE) &&
			    (step->value[0] == lst->name[0]) &&
			    (xmlStrEqual(step->value, lst->name)))
			    break;
			lst = lst->next;
		    }
		    if (lst != NULL)
			continue;
		}
		goto rollback;
	    }
            case XSLT_OP_ATTR:
		if (node->type != XML_ATTRIBUTE_NODE)
		    goto rollback;
		if (step->value != NULL) {
		    if (step->value[0] != node->name[0])
			goto rollback;
		    if (!xmlStrEqual(step->value, node->name))
			goto rollback;
		}
		/* Namespace test */
		if (node->ns == NULL) {
		    if (step->value2 != NULL)
			goto rollback;
		} else if (step->value2 != NULL) {
		    if (!xmlStrEqual(step->value2, node->ns->href))
			goto rollback;
		}
		continue;
            case XSLT_OP_PARENT:
		if ((node->type == XML_DOCUMENT_NODE) ||
		    (node->type == XML_HTML_DOCUMENT_NODE) ||
#ifdef LIBXML_DOCB_ENABLED
		    (node->type == XML_DOCB_DOCUMENT_NODE) ||
#endif
		    (node->type == XML_NAMESPACE_DECL))
		    goto rollback;
		node = node->parent;
		if (node == NULL)
		    goto rollback;
		if (step->value == NULL)
		    continue;
		if (step->value[0] != node->name[0])
		    goto rollback;
		if (!xmlStrEqual(step->value, node->name))
		    goto rollback;
		/* Namespace test */
		if (node->ns == NULL) {
		    if (step->value2 != NULL)
			goto rollback;
		} else if (node->ns->href != NULL) {
		    if (step->value2 == NULL)
			goto rollback;
		    if (!xmlStrEqual(step->value2, node->ns->href))
			goto rollback;
		}
		continue;
            case XSLT_OP_ANCESTOR:
		/* TODO: implement coalescing of ANCESTOR/NODE ops */
		if (step->value == NULL) {
		    step = &comp->steps[i+1];
		    if (step->op == XSLT_OP_ROOT)
			goto found;
		    /* added NS, ID and KEY as a result of bug 168208 */
		    if ((step->op != XSLT_OP_ELEM) && 
			(step->op != XSLT_OP_ALL) && 
			(step->op != XSLT_OP_NS) &&
			(step->op != XSLT_OP_ID) &&
			(step->op != XSLT_OP_KEY))
			goto rollback;
		}
		if (node == NULL)
		    goto rollback;
		if ((node->type == XML_DOCUMENT_NODE) ||
		    (node->type == XML_HTML_DOCUMENT_NODE) ||
#ifdef LIBXML_DOCB_ENABLED
		    (node->type == XML_DOCB_DOCUMENT_NODE) ||
#endif
		    (node->type == XML_NAMESPACE_DECL))
		    goto rollback;
		node = node->parent;
		if ((step->op != XSLT_OP_ELEM) && step->op != XSLT_OP_ALL) {
		    xsltPatPushState(&states, i, node);
		    continue;
		}
		i++;
		if (step->value == NULL) {
		    xsltPatPushState(&states, i - 1, node);
		    continue;
		}
		while (node != NULL) {
		    if ((node->type == XML_ELEMENT_NODE) &&
			(step->value[0] == node->name[0]) &&
			(xmlStrEqual(step->value, node->name))) {
			/* Namespace test */
			if (node->ns == NULL) {
			    if (step->value2 == NULL)
				break;
			} else if (node->ns->href != NULL) {
			    if ((step->value2 != NULL) &&
			        (xmlStrEqual(step->value2, node->ns->href)))
				break;
			}
		    }
		    node = node->parent;
		}
		if (node == NULL)
		    goto rollback;
		xsltPatPushState(&states, i - 1, node);
		continue;
            case XSLT_OP_ID: {
		/* TODO Handle IDs decently, must be done differently */
		xmlAttrPtr id;

		if (node->type != XML_ELEMENT_NODE)
		    goto rollback;

		id = xmlGetID(node->doc, step->value);
		if ((id == NULL) || (id->parent != node))
		    goto rollback;
		break;
	    }
            case XSLT_OP_KEY: {
		xmlNodeSetPtr list;
		int indx;

		list = xsltGetKey(ctxt, step->value,
			          step->value3, step->value2);
		if (list == NULL)
		    goto rollback;
		for (indx = 0;indx < list->nodeNr;indx++)
		    if (list->nodeTab[indx] == node)
			break;
		if (indx >= list->nodeNr)
		    goto rollback;
		break;
	    }
            case XSLT_OP_NS:
		if (node->type != XML_ELEMENT_NODE)
		    goto rollback;
		if (node->ns == NULL) {
		    if (step->value != NULL)
			goto rollback;
		} else if (node->ns->href != NULL) {
		    if (step->value == NULL)
			goto rollback;
		    if (!xmlStrEqual(step->value, node->ns->href))
			goto rollback;
		}
		break;
            case XSLT_OP_ALL:
		if (node->type != XML_ELEMENT_NODE)
		    goto rollback;
		break;
	    case XSLT_OP_PREDICATE: {
		xmlNodePtr oldNode;
		xmlDocPtr doc;
		int oldCS, oldCP;
		int pos = 0, len = 0;
		int isRVT;

		/*
		 * when there is cascading XSLT_OP_PREDICATE, then use a
		 * direct computation approach. It's not done directly
		 * at the beginning of the routine to filter out as much
		 * as possible this costly computation.
		 */
		if (comp->direct) {
		    if (states.states != NULL) {
			/* Free the rollback states */
			xmlFree(states.states);
		    }
		    return(xsltTestCompMatchDirect(ctxt, comp, node,
		    				   comp->nsList, comp->nsNr));
		}

		doc = node->doc;
		if (XSLT_IS_RES_TREE_FRAG(doc))
		    isRVT = 1;
		else
		    isRVT = 0;

		/*
		 * Depending on the last selection, one may need to
		 * recompute contextSize and proximityPosition.
		 */
		oldCS = ctxt->xpathCtxt->contextSize;
		oldCP = ctxt->xpathCtxt->proximityPosition;
		if ((sel != NULL) &&
		    (sel->op == XSLT_OP_ELEM) &&
		    (sel->value != NULL) &&
		    (node->type == XML_ELEMENT_NODE) &&
		    (node->parent != NULL)) {
		    xmlNodePtr previous;
		    int ix, nocache = 0;

		    previous = (xmlNodePtr)
			XSLT_RUNTIME_EXTRA(ctxt, sel->previousExtra, ptr);
		    ix = XSLT_RUNTIME_EXTRA(ctxt, sel->indexExtra, ival);
		    if ((previous != NULL) &&
			(previous->parent == node->parent)) {
			/*
			 * just walk back to adjust the index
			 */
			int indx = 0;
			xmlNodePtr sibling = node;

			while (sibling != NULL) {
			    if (sibling == previous)
				break;
			    if ((previous->type == XML_ELEMENT_NODE) &&
				(previous->name != NULL) &&
				(sibling->name != NULL) &&
				(previous->name[0] == sibling->name[0]) &&
				(xmlStrEqual(previous->name, sibling->name)))
			    {
				if ((sel->value2 == NULL) ||
				    ((sibling->ns != NULL) &&
				     (xmlStrEqual(sel->value2,
						  sibling->ns->href))))
				    indx++;
			    }
			    sibling = sibling->prev;
			}
			if (sibling == NULL) {
			    /* hum going backward in document order ... */
			    indx = 0;
			    sibling = node;
			    while (sibling != NULL) {
				if (sibling == previous)
				    break;
				if ((previous->type == XML_ELEMENT_NODE) &&
				    (previous->name != NULL) &&
				    (sibling->name != NULL) &&
				    (previous->name[0] == sibling->name[0]) &&
				    (xmlStrEqual(previous->name, sibling->name)))
				{
				    if ((sel->value2 == NULL) ||
					((sibling->ns != NULL) &&
					(xmlStrEqual(sel->value2,
					sibling->ns->href))))
				    {
					indx--;
				    }
				}
				sibling = sibling->next;
			    }
			}
			if (sibling != NULL) {
			    pos = ix + indx;
			    /*
			     * If the node is in a Value Tree we need to
			     * save len, but cannot cache the node!
			     * (bugs 153137 and 158840)
			     */
			    if (node->doc != NULL) {
				len = XSLT_RUNTIME_EXTRA(ctxt,
				        sel->lenExtra, ival);
				if (!isRVT) {
				    XSLT_RUNTIME_EXTRA(ctxt,
					sel->previousExtra, ptr) = node;
				    XSLT_RUNTIME_EXTRA(ctxt,
				        sel->indexExtra, ival) = pos;
				}
			    }
			    ix = pos;
			} else
			    pos = 0;
		    } else {
			/*
			 * recompute the index
			 */
			xmlNodePtr siblings = node->parent->children;
			xmlNodePtr parent = node->parent;

			while (siblings != NULL) {
			    if (siblings->type == XML_ELEMENT_NODE) {
				if (siblings == node) {
				    len++;
				    pos = len;
				} else if ((node->name != NULL) &&
					   (siblings->name != NULL) &&
				    (node->name[0] == siblings->name[0]) &&
				    (xmlStrEqual(node->name, siblings->name))) {
				    if ((sel->value2 == NULL) ||
					((siblings->ns != NULL) &&
					 (xmlStrEqual(sel->value2,
						      siblings->ns->href))))
					len++;
				}
			    }
			    siblings = siblings->next;
			}
			if ((parent == NULL) || (node->doc == NULL))
			    nocache = 1;
			else {
			    while (parent->parent != NULL)
				parent = parent->parent;
			    if (((parent->type != XML_DOCUMENT_NODE) &&
				 (parent->type != XML_HTML_DOCUMENT_NODE)) ||
				 (parent != (xmlNodePtr) node->doc))
				nocache = 1;
			}
		    }
		    if (pos != 0) {
			ctxt->xpathCtxt->contextSize = len;
			ctxt->xpathCtxt->proximityPosition = pos;
			/*
			 * If the node is in a Value Tree we cannot
			 * cache it !
			 */
			if ((!isRVT) && (node->doc != NULL) &&
			    (nocache == 0)) {
			    XSLT_RUNTIME_EXTRA(ctxt, sel->previousExtra, ptr) =
				node;
			    XSLT_RUNTIME_EXTRA(ctxt, sel->indexExtra, ival) =
				pos;
			    XSLT_RUNTIME_EXTRA(ctxt, sel->lenExtra, ival) =
				len;
			}
		    }
		} else if ((sel != NULL) && (sel->op == XSLT_OP_ALL) &&
			   (node->type == XML_ELEMENT_NODE)) {
		    xmlNodePtr previous;
		    int ix, nocache = 0;

		    previous = (xmlNodePtr)
			XSLT_RUNTIME_EXTRA(ctxt, sel->previousExtra, ptr);
		    ix = XSLT_RUNTIME_EXTRA(ctxt, sel->indexExtra, ival);
		    if ((previous != NULL) &&
			(previous->parent == node->parent)) {
			/*
			 * just walk back to adjust the index
			 */
			int indx = 0;
			xmlNodePtr sibling = node;

			while (sibling != NULL) {
			    if (sibling == previous)
				break;
			    if (sibling->type == XML_ELEMENT_NODE)
				indx++;
			    sibling = sibling->prev;
			}
			if (sibling == NULL) {
			    /* hum going backward in document order ... */
			    indx = 0;
			    sibling = node;
			    while (sibling != NULL) {
				if (sibling == previous)
				    break;
				if (sibling->type == XML_ELEMENT_NODE)
				    indx--;
				sibling = sibling->next;
			    }
			}
			if (sibling != NULL) {
			    pos = ix + indx;
			    /*
			     * If the node is in a Value Tree we cannot
			     * cache it !
			     */
			    if ((node->doc != NULL) && !isRVT) {
				len = XSLT_RUNTIME_EXTRA(ctxt,
				        sel->lenExtra, ival);
				XSLT_RUNTIME_EXTRA(ctxt,
					sel->previousExtra, ptr) = node;
				XSLT_RUNTIME_EXTRA(ctxt,
					sel->indexExtra, ival) = pos;
			    }
			} else
			    pos = 0;
		    } else {
			/*
			 * recompute the index
			 */
			xmlNodePtr siblings = node->parent->children;
			xmlNodePtr parent = node->parent;

			while (siblings != NULL) {
			    if (siblings->type == XML_ELEMENT_NODE) {
				len++;
				if (siblings == node) {
				    pos = len;
				}
			    }
			    siblings = siblings->next;
			}
			if ((parent == NULL) || (node->doc == NULL))
			    nocache = 1;
			else {
			    while (parent->parent != NULL)
				parent = parent->parent;
			    if (((parent->type != XML_DOCUMENT_NODE) &&
				 (parent->type != XML_HTML_DOCUMENT_NODE)) ||
				 (parent != (xmlNodePtr) node->doc))
				nocache = 1;
			}
		    }
		    if (pos != 0) {
			ctxt->xpathCtxt->contextSize = len;
			ctxt->xpathCtxt->proximityPosition = pos;
			/*
			 * If the node is in a Value Tree we cannot
			 * cache it !
			 */
			if ((node->doc != NULL) && (nocache == 0) && !isRVT) {
			    XSLT_RUNTIME_EXTRA(ctxt, sel->previousExtra, ptr) =
				node;
			    XSLT_RUNTIME_EXTRA(ctxt, sel->indexExtra, ival) =
				pos;
			    XSLT_RUNTIME_EXTRA(ctxt, sel->lenExtra, ival) =
				len;
			}
		    }
		}
		oldNode = ctxt->node;
		ctxt->node = node;

		if (step->value == NULL)
		    goto wrong_index;
		if (step->comp == NULL)
		    goto wrong_index;

		if (!xsltEvalXPathPredicate(ctxt, step->comp, comp->nsList,
			                    comp->nsNr))
		    goto wrong_index;

		if (pos != 0) {
		    ctxt->xpathCtxt->contextSize = oldCS;
		    ctxt->xpathCtxt->proximityPosition = oldCP;
		}
		ctxt->node = oldNode;
		break;
wrong_index:
		if (pos != 0) {
		    ctxt->xpathCtxt->contextSize = oldCS;
		    ctxt->xpathCtxt->proximityPosition = oldCP;
		}
		ctxt->node = oldNode;
		goto rollback;
	    }
            case XSLT_OP_PI:
		if (node->type != XML_PI_NODE)
		    goto rollback;
		if (step->value != NULL) {
		    if (!xmlStrEqual(step->value, node->name))
			goto rollback;
		}
		break;
            case XSLT_OP_COMMENT:
		if (node->type != XML_COMMENT_NODE)
		    goto rollback;
		break;
            case XSLT_OP_TEXT:
		if ((node->type != XML_TEXT_NODE) &&
		    (node->type != XML_CDATA_SECTION_NODE))
		    goto rollback;
		break;
            case XSLT_OP_NODE:
		switch (node->type) {
		    case XML_ELEMENT_NODE:
		    case XML_CDATA_SECTION_NODE:
		    case XML_PI_NODE:
		    case XML_COMMENT_NODE:
		    case XML_TEXT_NODE:
			break;
		    default:
			goto rollback;
		}
		break;
	}
    }
found:
    if (states.states != NULL) {
        /* Free the rollback states */
	xmlFree(states.states);
    }
    return(1);
rollback:
    /* got an error try to rollback */
    if (states.states == NULL)
	return(0);
    if (states.nbstates <= 0) {
	xmlFree(states.states);
	return(0);
    }
    states.nbstates--;
    i = states.states[states.nbstates].step;
    node = states.states[states.nbstates].node;
#if 0
    fprintf(stderr, "Pop: %d, %s\n", i, node->name);
#endif
    goto restart;
}

/**
 * xsltTestCompMatchList:
 * @ctxt:  a XSLT process context
 * @node: a node
 * @comp: the precompiled pattern list
 *
 * Test whether the node matches one of the patterns in the list
 *
 * Returns 1 if it matches, 0 if it doesn't and -1 in case of failure
 */
int
xsltTestCompMatchList(xsltTransformContextPtr ctxt, xmlNodePtr node,
	              xsltCompMatchPtr comp) {
    int ret;

    if ((ctxt == NULL) || (node == NULL))
	return(-1);
    while (comp != NULL) {
	ret = xsltTestCompMatch(ctxt, comp, node, NULL, NULL);
	if (ret == 1)
	    return(1);
	comp = comp->next;
    }
    return(0);
}

/************************************************************************
 *									*
 *			Dedicated parser for templates			*
 *									*
 ************************************************************************/

#define CUR (*ctxt->cur)
#define SKIP(val) ctxt->cur += (val)
#define NXT(val) ctxt->cur[(val)]
#define CUR_PTR ctxt->cur

#define SKIP_BLANKS 							\
    while (IS_BLANK_CH(CUR)) NEXT

#define CURRENT (*ctxt->cur)
#define NEXT ((*ctxt->cur) ?  ctxt->cur++: ctxt->cur)


#define PUSH(op, val, val2, novar) 						\
    if (xsltCompMatchAdd(ctxt, ctxt->comp, (op), (val), (val2), (novar))) goto error;

#define SWAP() 						\
    xsltSwapTopCompMatch(ctxt->comp);

#define XSLT_ERROR(X)							\
    { xsltError(ctxt, __FILE__, __LINE__, X);			\
      ctxt->error = (X); return; }

#define XSLT_ERROR0(X)							\
    { xsltError(ctxt, __FILE__, __LINE__, X);			\
      ctxt->error = (X); return(0); }

/**
 * xsltScanLiteral:
 * @ctxt:  the XPath Parser context
 *
 * Parse an XPath Litteral:
 *
 * [29] Literal ::= '"' [^"]* '"'
 *                | "'" [^']* "'"
 *
 * Returns the Literal parsed or NULL
 */

static xmlChar *
xsltScanLiteral(xsltParserContextPtr ctxt) {
    const xmlChar *q, *cur;
    xmlChar *ret = NULL;
    int val, len;

    SKIP_BLANKS;
    if (CUR == '"') {
        NEXT;
	cur = q = CUR_PTR;
	val = xmlStringCurrentChar(NULL, cur, &len);
	while ((IS_CHAR(val)) && (val != '"')) {
	    cur += len;
	    val = xmlStringCurrentChar(NULL, cur, &len);
	}
	if (!IS_CHAR(val)) {
	    ctxt->error = 1;
	    return(NULL);
	} else {
	    ret = xmlStrndup(q, cur - q);
        }
	cur += len;
	CUR_PTR = cur;
    } else if (CUR == '\'') {
        NEXT;
	cur = q = CUR_PTR;
	val = xmlStringCurrentChar(NULL, cur, &len);
	while ((IS_CHAR(val)) && (val != '\'')) {
	    cur += len;
	    val = xmlStringCurrentChar(NULL, cur, &len);
	}
	if (!IS_CHAR(val)) {
	    ctxt->error = 1;
	    return(NULL);
	} else {
	    ret = xmlStrndup(q, cur - q);
        }
	cur += len;
	CUR_PTR = cur;
    } else {
	/* XP_ERROR(XPATH_START_LITERAL_ERROR); */
	ctxt->error = 1;
	return(NULL);
    }
    return(ret);
}

/**
 * xsltScanName:
 * @ctxt:  the XPath Parser context
 *
 * [4] NameChar ::= Letter | Digit | '.' | '-' | '_' | 
 *                  CombiningChar | Extender
 *
 * [5] Name ::= (Letter | '_' | ':') (NameChar)*
 *
 * [6] Names ::= Name (S Name)*
 *
 * Returns the Name parsed or NULL
 */

static xmlChar *
xsltScanName(xsltParserContextPtr ctxt) {
    const xmlChar *q, *cur;
    xmlChar *ret = NULL;
    int val, len;

    SKIP_BLANKS;

    cur = q = CUR_PTR;
    val = xmlStringCurrentChar(NULL, cur, &len);
    if (!IS_LETTER(val) && (val != '_') && (val != ':'))
	return(NULL);

    while ((IS_LETTER(val)) || (IS_DIGIT(val)) ||
           (val == '.') || (val == '-') ||
	   (val == '_') || 
	   (IS_COMBINING(val)) ||
	   (IS_EXTENDER(val))) {
	cur += len;
	val = xmlStringCurrentChar(NULL, cur, &len);
    }
    ret = xmlStrndup(q, cur - q);
    CUR_PTR = cur;
    return(ret);
}

/**
 * xsltScanNCName:
 * @ctxt:  the XPath Parser context
 *
 * Parses a non qualified name
 *
 * Returns the Name parsed or NULL
 */

static xmlChar *
xsltScanNCName(xsltParserContextPtr ctxt) {
    const xmlChar *q, *cur;
    xmlChar *ret = NULL;
    int val, len;

    SKIP_BLANKS;

    cur = q = CUR_PTR;
    val = xmlStringCurrentChar(NULL, cur, &len);
    if (!IS_LETTER(val) && (val != '_'))
	return(NULL);

    while ((IS_LETTER(val)) || (IS_DIGIT(val)) ||
           (val == '.') || (val == '-') ||
	   (val == '_') ||
	   (IS_COMBINING(val)) ||
	   (IS_EXTENDER(val))) {
	cur += len;
	val = xmlStringCurrentChar(NULL, cur, &len);
    }
    ret = xmlStrndup(q, cur - q);
    CUR_PTR = cur;
    return(ret);
}

/**
 * xsltScanQName:
 * @ctxt:  the XPath Parser context
 * @prefix:  the place to store the prefix
 *
 * Parse a qualified name
 *
 * Returns the Name parsed or NULL
 */

static xmlChar *
xsltScanQName(xsltParserContextPtr ctxt, xmlChar **prefix) {
    xmlChar *ret = NULL;

    *prefix = NULL;
    ret = xsltScanNCName(ctxt);
    if (CUR == ':') {
        *prefix = ret;
	NEXT;
	ret = xsltScanNCName(ctxt);
    }
    return(ret);
}

/*
 * xsltCompileIdKeyPattern:
 * @ctxt:  the compilation context
 * @name:  a preparsed name
 * @aid:  whether id/key are allowed there
 * @novar:  flag to prohibit xslt var
 *
 * Compile the XSLT LocationIdKeyPattern
 * [3] IdKeyPattern ::= 'id' '(' Literal ')'
 *                    | 'key' '(' Literal ',' Literal ')'
 *
 * also handle NodeType and PI from:
 *
 * [7]  NodeTest ::= NameTest
 *                 | NodeType '(' ')'
 *                 | 'processing-instruction' '(' Literal ')'
 */
static void
xsltCompileIdKeyPattern(xsltParserContextPtr ctxt, xmlChar *name,
		int aid, int novar) {
    xmlChar *lit = NULL;
    xmlChar *lit2 = NULL;

    if (CUR != '(') {
	xsltTransformError(NULL, NULL, NULL,
		"xsltCompileIdKeyPattern : ( expected\n");
	ctxt->error = 1;
	return;
    }
    if ((aid) && (xmlStrEqual(name, (const xmlChar *)"id"))) {
	NEXT;
	SKIP_BLANKS;
        lit = xsltScanLiteral(ctxt);
	if (ctxt->error)
	    return;
	SKIP_BLANKS;
	if (CUR != ')') {
	    xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileIdKeyPattern : ) expected\n");
	    ctxt->error = 1;
	    return;
	}
	NEXT;
	PUSH(XSLT_OP_ID, lit, NULL, novar);
    } else if ((aid) && (xmlStrEqual(name, (const xmlChar *)"key"))) {
	NEXT;
	SKIP_BLANKS;
        lit = xsltScanLiteral(ctxt);
	if (ctxt->error)
	    return;
	SKIP_BLANKS;
	if (CUR != ',') {
	    xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileIdKeyPattern : , expected\n");
	    ctxt->error = 1;
	    return;
	}
	NEXT;
	SKIP_BLANKS;
        lit2 = xsltScanLiteral(ctxt);
	if (ctxt->error)
	    return;
	SKIP_BLANKS;
	if (CUR != ')') {
	    xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileIdKeyPattern : ) expected\n");
	    ctxt->error = 1;
	    return;
	}
	NEXT;
	/* URGENT TODO: support namespace in keys */
	PUSH(XSLT_OP_KEY, lit, lit2, novar);
    } else if (xmlStrEqual(name, (const xmlChar *)"processing-instruction")) {
	NEXT;
	SKIP_BLANKS;
	if (CUR != ')') {
	    lit = xsltScanLiteral(ctxt);
	    if (ctxt->error)
		return;
	    SKIP_BLANKS;
	    if (CUR != ')') {
		xsltTransformError(NULL, NULL, NULL,
			"xsltCompileIdKeyPattern : ) expected\n");
		ctxt->error = 1;
		return;
	    }
	}
	NEXT;
	PUSH(XSLT_OP_PI, lit, NULL, novar);
    } else if (xmlStrEqual(name, (const xmlChar *)"text")) {
	NEXT;
	SKIP_BLANKS;
	if (CUR != ')') {
	    xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileIdKeyPattern : ) expected\n");
	    ctxt->error = 1;
	    return;
	}
	NEXT;
	PUSH(XSLT_OP_TEXT, NULL, NULL, novar);
    } else if (xmlStrEqual(name, (const xmlChar *)"comment")) {
	NEXT;
	SKIP_BLANKS;
	if (CUR != ')') {
	    xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileIdKeyPattern : ) expected\n");
	    ctxt->error = 1;
	    return;
	}
	NEXT;
	PUSH(XSLT_OP_COMMENT, NULL, NULL, novar);
    } else if (xmlStrEqual(name, (const xmlChar *)"node")) {
	NEXT;
	SKIP_BLANKS;
	if (CUR != ')') {
	    xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileIdKeyPattern : ) expected\n");
	    ctxt->error = 1;
	    return;
	}
	NEXT;
	PUSH(XSLT_OP_NODE, NULL, NULL, novar);
    } else if (aid) {
	xsltTransformError(NULL, NULL, NULL,
	    "xsltCompileIdKeyPattern : expecting 'key' or 'id' or node type\n");
	ctxt->error = 1;
	return;
    } else {
	xsltTransformError(NULL, NULL, NULL,
	    "xsltCompileIdKeyPattern : node type\n");
	ctxt->error = 1;
	return;
    }
error:
    if (name != NULL)
	xmlFree(name);
}

/**
 * xsltCompileStepPattern:
 * @ctxt:  the compilation context
 * @token:  a posible precompiled name
 * @novar: flag to prohibit xslt variables from pattern
 *
 * Compile the XSLT StepPattern and generates a precompiled
 * form suitable for fast matching.
 *
 * [5] StepPattern ::= ChildOrAttributeAxisSpecifier NodeTest Predicate* 
 * [6] ChildOrAttributeAxisSpecifier ::= AbbreviatedAxisSpecifier
 *                                     | ('child' | 'attribute') '::'
 * from XPath
 * [7]  NodeTest ::= NameTest
 *                 | NodeType '(' ')'
 *                 | 'processing-instruction' '(' Literal ')'
 * [8] Predicate ::= '[' PredicateExpr ']'
 * [9] PredicateExpr ::= Expr
 * [13] AbbreviatedAxisSpecifier ::= '@'?
 * [37] NameTest ::= '*' | NCName ':' '*' | QName
 */

static void
xsltCompileStepPattern(xsltParserContextPtr ctxt, xmlChar *token, int novar) {
    xmlChar *name = NULL;
    const xmlChar *URI = NULL;
    xmlChar *URL = NULL;
    int level;

    SKIP_BLANKS;
    if ((token == NULL) && (CUR == '@')) {
	xmlChar *prefix = NULL;

	NEXT;
	if (CUR == '*') {
	    NEXT;
	    PUSH(XSLT_OP_ATTR, NULL, NULL, novar);
	    goto parse_predicate;
	}
	token = xsltScanQName(ctxt, &prefix);
	if (prefix != NULL) {
	    xmlNsPtr ns;

	    ns = xmlSearchNs(ctxt->doc, ctxt->elem, prefix);
	    if (ns == NULL) {
		xsltTransformError(NULL, NULL, NULL,
		"xsltCompileStepPattern : no namespace bound to prefix %s\n",
				 prefix);
	    } else {
		URL = xmlStrdup(ns->href);
	    }
	    xmlFree(prefix);
	}
	if (token == NULL) {
	    if (CUR == '*') {
		NEXT;
		PUSH(XSLT_OP_ATTR, NULL, URL, novar);
		return;
	    }
	    xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileStepPattern : Name expected\n");
	    ctxt->error = 1;
	    goto error;
	}
	PUSH(XSLT_OP_ATTR, token, URL, novar);
	goto parse_predicate;
    }
    if (token == NULL)
	token = xsltScanName(ctxt);
    if (token == NULL) {
	if (CUR == '*') {
	    NEXT;
	    PUSH(XSLT_OP_ALL, token, NULL, novar);
	    goto parse_predicate;
	} else {
	    xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileStepPattern : Name expected\n");
	    ctxt->error = 1;
	    goto error;
	}
    }


    SKIP_BLANKS;
    if (CUR == '(') {
	xsltCompileIdKeyPattern(ctxt, token, 0, novar);
	if (ctxt->error)
	    goto error;
    } else if (CUR == ':') {
	NEXT;
	if (CUR != ':') {
	    xmlChar *prefix = token;
	    xmlNsPtr ns;

	    /*
	     * This is a namespace match
	     */
	    token = xsltScanName(ctxt);
	    ns = xmlSearchNs(ctxt->doc, ctxt->elem, prefix);
	    if (ns == NULL) {
		xsltTransformError(NULL, NULL, NULL,
	    "xsltCompileStepPattern : no namespace bound to prefix %s\n",
				 prefix);
		ctxt->error = 1;
		goto error;
	    } else {
		URL = xmlStrdup(ns->href);
	    }
	    xmlFree(prefix);
	    if (token == NULL) {
		if (CUR == '*') {
		    NEXT;
		    PUSH(XSLT_OP_NS, URL, NULL, novar);
		} else {
		    xsltTransformError(NULL, NULL, NULL,
			    "xsltCompileStepPattern : Name expected\n");
		    ctxt->error = 1;
		    goto error;
		}
	    } else {
		PUSH(XSLT_OP_ELEM, token, URL, novar);
	    }
	} else {
	    NEXT;
	    if (xmlStrEqual(token, (const xmlChar *) "child")) {
		xmlFree(token);
		token = xsltScanName(ctxt);
		if (token == NULL) {
	            if (CUR == '*') {
            	        NEXT;
	                PUSH(XSLT_OP_ALL, token, NULL, novar);
	                goto parse_predicate;
	            } else {
		        xsltTransformError(NULL, NULL, NULL,
			    "xsltCompileStepPattern : QName expected\n");
		        ctxt->error = 1;
		        goto error;
		    }
		}
		URI = xsltGetQNameURI(ctxt->elem, &token);
		if (token == NULL) {
		    ctxt->error = 1;
		    goto error;
		} else {
		    name = xmlStrdup(token);
		    if (URI != NULL)
			URL = xmlStrdup(URI);
		}
		PUSH(XSLT_OP_CHILD, name, URL, novar);
	    } else if (xmlStrEqual(token, (const xmlChar *) "attribute")) {
		xmlFree(token);
		token = xsltScanName(ctxt);
		if (token == NULL) {
		    xsltTransformError(NULL, NULL, NULL,
			    "xsltCompileStepPattern : QName expected\n");
		    ctxt->error = 1;
		    goto error;
		}
		URI = xsltGetQNameURI(ctxt->elem, &token);
		if (token == NULL) {
		    ctxt->error = 1;
		    goto error;
		} else {
		    name = xmlStrdup(token);
		    if (URI != NULL)
			URL = xmlStrdup(URI);
		}
		PUSH(XSLT_OP_ATTR, name, URL, novar);
	    } else {
		xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileStepPattern : 'child' or 'attribute' expected\n");
		ctxt->error = 1;
		goto error;
	    }
	    xmlFree(token);
	}
    } else if (CUR == '*') {
	NEXT;
	PUSH(XSLT_OP_ALL, token, NULL, novar);
    } else {
	URI = xsltGetQNameURI(ctxt->elem, &token);
	if (token == NULL) {
	    ctxt->error = 1;
	    goto error;
	}
	if (URI != NULL)
	    URL = xmlStrdup(URI);
	PUSH(XSLT_OP_ELEM, token, URL, novar);
    }
parse_predicate:
    SKIP_BLANKS;
    level = 0;
    while (CUR == '[') {
	const xmlChar *q;
	xmlChar *ret = NULL;

	level++;
	NEXT;
	q = CUR_PTR;
	while (CUR != 0) {
	    /* Skip over nested predicates */
	    if (CUR == '[')
		level++;
	    else if (CUR == ']') {
		level--;
		if (level == 0)
		    break;
	    } else if (CUR == '"') {
		NEXT;
		while ((CUR != 0) && (CUR != '"'))
		    NEXT;
	    } else if (CUR == '\'') {
		NEXT;
		while ((CUR != 0) && (CUR != '\''))
		    NEXT;
	    }
	    NEXT;
	}
	if (CUR == 0) {
	    xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileStepPattern : ']' expected\n");
	    ctxt->error = 1;
	    return;
        }
	ret = xmlStrndup(q, CUR_PTR - q);
	PUSH(XSLT_OP_PREDICATE, ret, NULL, novar);
	/* push the predicate lower than local test */
	SWAP();
	NEXT;
	SKIP_BLANKS;
    }
    return;
error:
    if (token != NULL)
	xmlFree(token);
    if (name != NULL)
	xmlFree(name);
}

/**
 * xsltCompileRelativePathPattern:
 * @comp:  the compilation context
 * @token:  a posible precompiled name
 * @novar:  flag to prohibit xslt variables
 *
 * Compile the XSLT RelativePathPattern and generates a precompiled
 * form suitable for fast matching.
 *
 * [4] RelativePathPattern ::= StepPattern
 *                           | RelativePathPattern '/' StepPattern
 *                           | RelativePathPattern '//' StepPattern
 */
static void
xsltCompileRelativePathPattern(xsltParserContextPtr ctxt, xmlChar *token, int novar) {
    xsltCompileStepPattern(ctxt, token, novar);
    if (ctxt->error)
	goto error;
    SKIP_BLANKS;
    while ((CUR != 0) && (CUR != '|')) {
	if ((CUR == '/') && (NXT(1) == '/')) {
	    PUSH(XSLT_OP_ANCESTOR, NULL, NULL, novar);
	    NEXT;
	    NEXT;
	    SKIP_BLANKS;
	    xsltCompileStepPattern(ctxt, NULL, novar);
	} else if (CUR == '/') {
	    PUSH(XSLT_OP_PARENT, NULL, NULL, novar);
	    NEXT;
	    SKIP_BLANKS;
	    if ((CUR != 0) && (CUR != '|')) {
		xsltCompileRelativePathPattern(ctxt, NULL, novar);
	    }
	} else {
	    ctxt->error = 1;
	}
	if (ctxt->error)
	    goto error;
	SKIP_BLANKS;
    }
error:
    return;
}

/**
 * xsltCompileLocationPathPattern:
 * @ctxt:  the compilation context
 * @novar:  flag to prohibit xslt variables
 *
 * Compile the XSLT LocationPathPattern and generates a precompiled
 * form suitable for fast matching.
 *
 * [2] LocationPathPattern ::= '/' RelativePathPattern?
 *                           | IdKeyPattern (('/' | '//') RelativePathPattern)?
 *                           | '//'? RelativePathPattern
 */
static void
xsltCompileLocationPathPattern(xsltParserContextPtr ctxt, int novar) {
    SKIP_BLANKS;
    if ((CUR == '/') && (NXT(1) == '/')) {
	/*
	 * since we reverse the query
	 * a leading // can be safely ignored
	 */
	NEXT;
	NEXT;
	ctxt->comp->priority = 0.5;	/* '//' means not 0 priority */
	xsltCompileRelativePathPattern(ctxt, NULL, novar);
    } else if (CUR == '/') {
	/*
	 * We need to find root as the parent
	 */
	NEXT;
	SKIP_BLANKS;
	PUSH(XSLT_OP_ROOT, NULL, NULL, novar);
	if ((CUR != 0) && (CUR != '|')) {
	    PUSH(XSLT_OP_PARENT, NULL, NULL, novar);
	    xsltCompileRelativePathPattern(ctxt, NULL, novar);
	}
    } else if (CUR == '*') {
	xsltCompileRelativePathPattern(ctxt, NULL, novar);
    } else if (CUR == '@') {
	xsltCompileRelativePathPattern(ctxt, NULL, novar);
    } else {
	xmlChar *name;
	name = xsltScanName(ctxt);
	if (name == NULL) {
	    xsltTransformError(NULL, NULL, NULL,
		    "xsltCompileLocationPathPattern : Name expected\n");
	    ctxt->error = 1;
	    return;
	}
	SKIP_BLANKS;
	if ((CUR == '(') && !xmlXPathIsNodeType(name)) {
	    xsltCompileIdKeyPattern(ctxt, name, 1, novar);
	    if ((CUR == '/') && (NXT(1) == '/')) {
		PUSH(XSLT_OP_ANCESTOR, NULL, NULL, novar);
		NEXT;
		NEXT;
		SKIP_BLANKS;
		xsltCompileRelativePathPattern(ctxt, NULL, novar);
	    } else if (CUR == '/') {
		PUSH(XSLT_OP_PARENT, NULL, NULL, novar);
		NEXT;
		SKIP_BLANKS;
		xsltCompileRelativePathPattern(ctxt, NULL, novar);
	    }
	    return;
	}
	xsltCompileRelativePathPattern(ctxt, name, novar);
    }
error:
    return;
}

/**
 * xsltCompilePatternInternal:
 * @pattern: an XSLT pattern
 * @doc:  the containing document
 * @node:  the containing element
 * @style:  the stylesheet
 * @runtime:  the transformation context, if done at run-time
 * @novar:  flag to prohibit xslt variables
 *
 * Compile the XSLT pattern and generates a list of precompiled form suitable
 * for fast matching.
 *
 * [1] Pattern ::= LocationPathPattern | Pattern '|' LocationPathPattern
 *
 * Returns the generated pattern list or NULL in case of failure
 */

static xsltCompMatchPtr
xsltCompilePatternInternal(const xmlChar *pattern, xmlDocPtr doc,
	           xmlNodePtr node, xsltStylesheetPtr style,
		   xsltTransformContextPtr runtime, int novar) {
    xsltParserContextPtr ctxt = NULL;
    xsltCompMatchPtr element, first = NULL, previous = NULL;
    int current, start, end, level, j;

    if (pattern == NULL) {
	xsltTransformError(NULL, NULL, node,
			 "xsltCompilePattern : NULL pattern\n");
	return(NULL);
    }

    ctxt = xsltNewParserContext(style, runtime);
    if (ctxt == NULL)
	return(NULL);
    ctxt->doc = doc;
    ctxt->elem = node;
    current = end = 0;
    while (pattern[current] != 0) {
	start = current;
	while (IS_BLANK_CH(pattern[current]))
	    current++;
	end = current;
	level = 0;
	while ((pattern[end] != 0) && ((pattern[end] != '|') || (level != 0))) {
	    if (pattern[end] == '[')
		level++;
	    else if (pattern[end] == ']')
		level--;
	    else if (pattern[end] == '\'') {
		end++;
		while ((pattern[end] != 0) && (pattern[end] != '\''))
		    end++;
	    } else if (pattern[end] == '"') {
		end++;
		while ((pattern[end] != 0) && (pattern[end] != '"'))
		    end++;
	    }
	    end++;
	}
	if (current == end) {
	    xsltTransformError(NULL, NULL, node,
			     "xsltCompilePattern : NULL pattern\n");
	    goto error;
	}
	element = xsltNewCompMatch();
	if (element == NULL) {
	    goto error;
	}
	if (first == NULL)
	    first = element;
	else if (previous != NULL)
	    previous->next = element;
	previous = element;

	ctxt->comp = element;
	ctxt->base = xmlStrndup(&pattern[start], end - start);
	if (ctxt->base == NULL)
	    goto error;
	ctxt->cur = &(ctxt->base)[current - start];
	element->pattern = ctxt->base;
	element->nsList = xmlGetNsList(doc, node);
	j = 0;
	if (element->nsList != NULL) {
	    while (element->nsList[j] != NULL)
		j++;
	}
	element->nsNr = j;


#ifdef WITH_XSLT_DEBUG_PATTERN
	xsltGenericDebug(xsltGenericDebugContext,
			 "xsltCompilePattern : parsing '%s'\n",
			 element->pattern);
#endif
	/*
	 Preset default priority to be zero.
	 This may be changed by xsltCompileLocationPathPattern.
	 */
	element->priority = 0;
	xsltCompileLocationPathPattern(ctxt, novar);
	if (ctxt->error) {
	    xsltTransformError(NULL, style, node,
			     "xsltCompilePattern : failed to compile '%s'\n",
			     element->pattern);
	    if (style != NULL) style->errors++;
	    goto error;
	}

	/*
	 * Reverse for faster interpretation.
	 */
	xsltReverseCompMatch(element);

	/*
	 * Set-up the priority
	 */
	if (element->priority == 0) {	/* if not yet determined */
	    if (((element->steps[0].op == XSLT_OP_ELEM) ||
		 (element->steps[0].op == XSLT_OP_ATTR) ||
		 (element->steps[0].op == XSLT_OP_PI)) &&
		(element->steps[0].value != NULL) &&
		(element->steps[1].op == XSLT_OP_END)) {
		;	/* previously preset */
	    } else if ((element->steps[0].op == XSLT_OP_ATTR) &&
		       (element->steps[0].value2 != NULL) &&
		       (element->steps[1].op == XSLT_OP_END)) {
			element->priority = -0.25;
	    } else if ((element->steps[0].op == XSLT_OP_NS) &&
		       (element->steps[0].value != NULL) &&
		       (element->steps[1].op == XSLT_OP_END)) {
			element->priority = -0.25;
	    } else if ((element->steps[0].op == XSLT_OP_ATTR) &&
		       (element->steps[0].value == NULL) &&
		       (element->steps[0].value2 == NULL) &&
		       (element->steps[1].op == XSLT_OP_END)) {
			element->priority = -0.5;
	    } else if (((element->steps[0].op == XSLT_OP_PI) ||
		       (element->steps[0].op == XSLT_OP_TEXT) ||
		       (element->steps[0].op == XSLT_OP_ALL) ||
		       (element->steps[0].op == XSLT_OP_NODE) ||
		       (element->steps[0].op == XSLT_OP_COMMENT)) &&
		       (element->steps[1].op == XSLT_OP_END)) {
			element->priority = -0.5;
	    } else {
		element->priority = 0.5;
	    }
	}
#ifdef WITH_XSLT_DEBUG_PATTERN
	xsltGenericDebug(xsltGenericDebugContext,
		     "xsltCompilePattern : parsed %s, default priority %f\n",
			 element->pattern, element->priority);
#endif
	if (pattern[end] == '|')
	    end++;
	current = end;
    }
    if (end == 0) {
	xsltTransformError(NULL, style, node,
			 "xsltCompilePattern : NULL pattern\n");
	if (style != NULL) style->errors++;
	goto error;
    }

    xsltFreeParserContext(ctxt);
    return(first);

error:
    if (ctxt != NULL)
	xsltFreeParserContext(ctxt);
    if (first != NULL)
	xsltFreeCompMatchList(first);
    return(NULL);
}

/**
 * xsltCompilePattern:
 * @pattern: an XSLT pattern
 * @doc:  the containing document
 * @node:  the containing element
 * @style:  the stylesheet
 * @runtime:  the transformation context, if done at run-time
 *
 * Compile the XSLT pattern and generates a list of precompiled form suitable
 * for fast matching.
 *
 * [1] Pattern ::= LocationPathPattern | Pattern '|' LocationPathPattern
 *
 * Returns the generated pattern list or NULL in case of failure
 */

xsltCompMatchPtr
xsltCompilePattern(const xmlChar *pattern, xmlDocPtr doc,
	           xmlNodePtr node, xsltStylesheetPtr style,
		   xsltTransformContextPtr runtime) {
    return (xsltCompilePatternInternal(pattern, doc, node, style, runtime, 0));
}

/************************************************************************
 *									*
 *			Module interfaces				*
 *									*
 ************************************************************************/

/**
 * xsltAddTemplate:
 * @style: an XSLT stylesheet
 * @cur: an XSLT template
 * @mode:  the mode name or NULL
 * @modeURI:  the mode URI or NULL
 *
 * Register the XSLT pattern associated to @cur
 *
 * Returns -1 in case of error, 0 otherwise
 */
int
xsltAddTemplate(xsltStylesheetPtr style, xsltTemplatePtr cur,
	        const xmlChar *mode, const xmlChar *modeURI) {
    xsltCompMatchPtr pat, list, next;
    /*
     * 'top' will point to style->xxxMatch ptr - declaring as 'void'
     *  avoids gcc 'type-punned pointer' warning.
     */
    void **top = NULL;
    const xmlChar *name = NULL;
    float priority;              /* the priority */

    if ((style == NULL) || (cur == NULL) || (cur->match == NULL))
	return(-1);

    priority = cur->priority;
    pat = xsltCompilePatternInternal(cur->match, style->doc, cur->elem,
		    style, NULL, 1);
    if (pat == NULL)
    	return(-1);
    while (pat) {
	next = pat->next;
	pat->next = NULL;
	name = NULL;
	
	pat->template = cur;
	if (mode != NULL)
	    pat->mode = xmlDictLookup(style->dict, mode, -1);
	if (modeURI != NULL)
	    pat->modeURI = xmlDictLookup(style->dict, modeURI, -1);
	if (priority != XSLT_PAT_NO_PRIORITY)
	    pat->priority = priority;

	/*
	 * insert it in the hash table list corresponding to its lookup name
	 */
	switch (pat->steps[0].op) {
        case XSLT_OP_ATTR:
	    if (pat->steps[0].value != NULL)
		name = pat->steps[0].value;
	    else
		top = &(style->attrMatch);
	    break;
        case XSLT_OP_CHILD:
        case XSLT_OP_PARENT:
        case XSLT_OP_ANCESTOR:
	    top = &(style->elemMatch);
	    break;
        case XSLT_OP_ROOT:
	    top = &(style->rootMatch);
	    break;
        case XSLT_OP_KEY:
	    top = &(style->keyMatch);
	    break;
        case XSLT_OP_ID:
	    /* TODO optimize ID !!! */
        case XSLT_OP_NS:
        case XSLT_OP_ALL:
	    top = &(style->elemMatch);
	    break;
        case XSLT_OP_END:
	case XSLT_OP_PREDICATE:
	    xsltTransformError(NULL, style, NULL,
			     "xsltAddTemplate: invalid compiled pattern\n");
	    xsltFreeCompMatch(pat);
	    return(-1);
	    /*
	     * TODO: some flags at the top level about type based patterns
	     *       would be faster than inclusion in the hash table.
	     */
	case XSLT_OP_PI:
	    if (pat->steps[0].value != NULL)
		name = pat->steps[0].value;
	    else
		top = &(style->piMatch);
	    break;
	case XSLT_OP_COMMENT:
	    top = &(style->commentMatch);
	    break;
	case XSLT_OP_TEXT:
	    top = &(style->textMatch);
	    break;
        case XSLT_OP_ELEM:
	case XSLT_OP_NODE:
	    if (pat->steps[0].value != NULL)
		name = pat->steps[0].value;
	    else
		top = &(style->elemMatch);
	    break;
	}
	if (name != NULL) {
	    if (style->templatesHash == NULL) {
		style->templatesHash = xmlHashCreate(1024);
		if (style->templatesHash == NULL) {
		    xsltFreeCompMatch(pat);
		    return(-1);
		}
		xmlHashAddEntry3(style->templatesHash, name, mode, modeURI, pat);
	    } else {
		list = (xsltCompMatchPtr) xmlHashLookup3(style->templatesHash,
							 name, mode, modeURI);
		if (list == NULL) {
		    xmlHashAddEntry3(style->templatesHash, name,
				     mode, modeURI, pat);
		} else {
		    /*
		     * Note '<=' since one must choose among the matching
		     * template rules that are left, the one that occurs
		     * last in the stylesheet
		     */
		    if (list->priority <= pat->priority) {
			pat->next = list;
			xmlHashUpdateEntry3(style->templatesHash, name,
					    mode, modeURI, pat, NULL);
		    } else {
			while (list->next != NULL) {
			    if (list->next->priority <= pat->priority)
				break;
			    list = list->next;
			}
			pat->next = list->next;
			list->next = pat;
		    }
		}
	    }
	} else if (top != NULL) {
	    list = *top;
	    if (list == NULL) {
		*top = pat;
		pat->next = NULL;
	    } else if (list->priority <= pat->priority) {
		pat->next = list;
		*top = pat;
	    } else {
		while (list->next != NULL) {
		    if (list->next->priority <= pat->priority)
			break;
		    list = list->next;
		}
		pat->next = list->next;
		list->next = pat;
	    }
	} else {
	    xsltTransformError(NULL, style, NULL,
			     "xsltAddTemplate: invalid compiled pattern\n");
	    xsltFreeCompMatch(pat);
	    return(-1);
	}
#ifdef WITH_XSLT_DEBUG_PATTERN
	if (mode)
	    xsltGenericDebug(xsltGenericDebugContext,
			 "added pattern : '%s' mode '%s' priority %f\n",
			     pat->pattern, pat->mode, pat->priority);
	else
	    xsltGenericDebug(xsltGenericDebugContext,
			 "added pattern : '%s' priority %f\n",
			     pat->pattern, pat->priority);
#endif

	pat = next;
    }
    return(0);
}

#ifdef XSLT_REFACTORED_KEYCOMP
static int
xsltComputeAllKeys(xsltTransformContextPtr ctxt, xmlNodePtr contextNode)
{
    if ((ctxt == NULL) || (contextNode == NULL)) {
	xsltTransformError(ctxt, NULL, ctxt->inst,
	    "Internal error in xsltComputeAllKeys(): "
	    "Bad arguments.\n");
	return(-1);
    }
    
    if (ctxt->document == NULL) {
	/*
	* The document info will only be NULL if we have a RTF.
	*/
	if (contextNode->doc->_private != NULL)
	    goto doc_info_mismatch;
	/*
	* On-demand creation of the document info (needed for keys).
	*/
	ctxt->document = xsltNewDocument(ctxt, contextNode->doc);
	if (ctxt->document == NULL)
	    return(-1);
    }
    return xsltInitAllDocKeys(ctxt);

doc_info_mismatch:
    xsltTransformError(ctxt, NULL, ctxt->inst,
	"Internal error in xsltComputeAllKeys(): "
	"The context's document info doesn't match the "
	"document info of the current result tree.\n");
    ctxt->state = XSLT_STATE_STOPPED;
    return(-1);
}
#endif

/**
 * xsltGetTemplate:
 * @ctxt:  a XSLT process context
 * @node:  the node being processed
 * @style:  the current style
 *
 * Finds the template applying to this node, if @style is non-NULL
 * it means one needs to look for the next imported template in scope.
 *
 * Returns the xsltTemplatePtr or NULL if not found
 */
xsltTemplatePtr
xsltGetTemplate(xsltTransformContextPtr ctxt, xmlNodePtr node,
	        xsltStylesheetPtr style)
{
    xsltStylesheetPtr curstyle;
    xsltTemplatePtr ret = NULL;
    const xmlChar *name = NULL;
    xsltCompMatchPtr list = NULL;
    float priority;
    int keyed = 0;

    if ((ctxt == NULL) || (node == NULL))
	return(NULL);

    if (style == NULL) {
	curstyle = ctxt->style;
    } else {
	curstyle = xsltNextImport(style);
    }

    while ((curstyle != NULL) && (curstyle != style)) {
	priority = XSLT_PAT_NO_PRIORITY;
	/* TODO : handle IDs/keys here ! */
	if (curstyle->templatesHash != NULL) {
	    /*
	     * Use the top name as selector
	     */
	    switch (node->type) {
		case XML_ELEMENT_NODE:
		    if (node->name[0] == ' ')
			break;
		case XML_ATTRIBUTE_NODE:
		case XML_PI_NODE:
		    name = node->name;
		    break;
		case XML_DOCUMENT_NODE:
		case XML_HTML_DOCUMENT_NODE:
		case XML_TEXT_NODE:
		case XML_CDATA_SECTION_NODE:
		case XML_COMMENT_NODE:
		case XML_ENTITY_REF_NODE:
		case XML_ENTITY_NODE:
		case XML_DOCUMENT_TYPE_NODE:
		case XML_DOCUMENT_FRAG_NODE:
		case XML_NOTATION_NODE:
		case XML_DTD_NODE:
		case XML_ELEMENT_DECL:
		case XML_ATTRIBUTE_DECL:
		case XML_ENTITY_DECL:
		case XML_NAMESPACE_DECL:
		case XML_XINCLUDE_START:
		case XML_XINCLUDE_END:
		    break;
		default:
		    return(NULL);

	    }
	}
	if (name != NULL) {
	    /*
	     * find the list of applicable expressions based on the name
	     */
	    list = (xsltCompMatchPtr) xmlHashLookup3(curstyle->templatesHash,
					     name, ctxt->mode, ctxt->modeURI);
	} else
	    list = NULL;
	while (list != NULL) {
	    if (xsltTestCompMatch(ctxt, list, node,
			          ctxt->mode, ctxt->modeURI)) {
		ret = list->template;
		priority = list->priority;
		break;
	    }
	    list = list->next;
	}
	list = NULL;

	/*
	 * find alternate generic matches
	 */
	switch (node->type) {
	    case XML_ELEMENT_NODE:
		if (node->name[0] == ' ')
		    list = curstyle->rootMatch;
		else
		    list = curstyle->elemMatch;
		if (node->psvi != NULL) keyed = 1;
		break;
	    case XML_ATTRIBUTE_NODE: {
	        xmlAttrPtr attr;

		list = curstyle->attrMatch;
		attr = (xmlAttrPtr) node;
		if (attr->psvi != NULL) keyed = 1;
		break;
	    }
	    case XML_PI_NODE:
		list = curstyle->piMatch;
		if (node->psvi != NULL) keyed = 1;
		break;
	    case XML_DOCUMENT_NODE:
	    case XML_HTML_DOCUMENT_NODE: {
	        xmlDocPtr doc;

		list = curstyle->rootMatch;
		doc = (xmlDocPtr) node;
		if (doc->psvi != NULL) keyed = 1;
		break;
	    }
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
		list = curstyle->textMatch;
		if (node->psvi != NULL) keyed = 1;
		break;
	    case XML_COMMENT_NODE:
		list = curstyle->commentMatch;
		if (node->psvi != NULL) keyed = 1;
		break;
	    case XML_ENTITY_REF_NODE:
	    case XML_ENTITY_NODE:
	    case XML_DOCUMENT_TYPE_NODE:
	    case XML_DOCUMENT_FRAG_NODE:
	    case XML_NOTATION_NODE:
	    case XML_DTD_NODE:
	    case XML_ELEMENT_DECL:
	    case XML_ATTRIBUTE_DECL:
	    case XML_ENTITY_DECL:
	    case XML_NAMESPACE_DECL:
	    case XML_XINCLUDE_START:
	    case XML_XINCLUDE_END:
		break;
	    default:
		break;
	}
	while ((list != NULL) &&
	       ((ret == NULL)  || (list->priority > priority))) {
	    if (xsltTestCompMatch(ctxt, list, node,
			          ctxt->mode, ctxt->modeURI)) {
		ret = list->template;
		priority = list->priority;
		break;
	    }
	    list = list->next;
	}
	/*
	 * Some of the tests for elements can also apply to documents
	 */
	if ((node->type == XML_DOCUMENT_NODE) ||
	    (node->type == XML_HTML_DOCUMENT_NODE) ||
	    (node->type == XML_TEXT_NODE)) {
	    list = curstyle->elemMatch;
	    while ((list != NULL) &&
		   ((ret == NULL)  || (list->priority > priority))) {
		if (xsltTestCompMatch(ctxt, list, node,
				      ctxt->mode, ctxt->modeURI)) {
		    ret = list->template;
		    priority = list->priority;
		    break;
		}
		list = list->next;
	    }
	} else if ((node->type == XML_PI_NODE) ||
		   (node->type == XML_COMMENT_NODE)) {
	    list = curstyle->elemMatch;
	    while ((list != NULL) &&
		   ((ret == NULL)  || (list->priority > priority))) {
		if (xsltTestCompMatch(ctxt, list, node,
				      ctxt->mode, ctxt->modeURI)) {
		    ret = list->template;
		    priority = list->priority;
		    break;
		}
		list = list->next;
	    }
	}

#ifdef XSLT_REFACTORED_KEYCOMP
keyed_match:
#endif
	if (keyed) {
	    list = curstyle->keyMatch;
	    while ((list != NULL) &&
		   ((ret == NULL)  || (list->priority > priority))) {
		if (xsltTestCompMatch(ctxt, list, node,
				      ctxt->mode, ctxt->modeURI)) {
		    ret = list->template;
		    priority = list->priority;
		    break;
		}
		list = list->next;
	    }
	}
#ifdef XSLT_REFACTORED_KEYCOMP	
	else if (ctxt->hasTemplKeyPatterns &&
	    ((ctxt->document == NULL) ||
	     (ctxt->document->nbKeysComputed < ctxt->nbKeys)))
	{
	    /*
	    * Compute all remaining keys for this document.
	    *
	    * REVISIT TODO: I think this could be further optimized.
	    */
	    if (xsltComputeAllKeys(ctxt, node) == -1)
		goto error;

	    switch (node->type) {
		case XML_ELEMENT_NODE:		    
		    if (node->psvi != NULL) keyed = 1;
		    break;
		case XML_ATTRIBUTE_NODE:
		    if (((xmlAttrPtr) node)->psvi != NULL) keyed = 1;
		    break;
		case XML_TEXT_NODE:
		case XML_CDATA_SECTION_NODE:
		case XML_COMMENT_NODE:
		case XML_PI_NODE:		
		    if (node->psvi != NULL) keyed = 1;
		    break;
		case XML_DOCUMENT_NODE:
		case XML_HTML_DOCUMENT_NODE:
		    if (((xmlDocPtr) node)->psvi != NULL) keyed = 1;
		    break;		
		default:
		    break;
	    }
	    if (keyed)
		goto keyed_match;
	}
#endif /* XSLT_REFACTORED_KEYCOMP */
	if (ret != NULL)
	    return(ret);

	/*
	 * Cycle on next curstylesheet import.
	 */
	curstyle = xsltNextImport(curstyle);
    }

error:
    return(NULL);
}

/**
 * xsltCleanupTemplates:
 * @style: an XSLT stylesheet
 *
 * Cleanup the state of the templates used by the stylesheet and
 * the ones it imports.
 */
void
xsltCleanupTemplates(xsltStylesheetPtr style ATTRIBUTE_UNUSED) {
}

/**
 * xsltFreeTemplateHashes:
 * @style: an XSLT stylesheet
 *
 * Free up the memory used by xsltAddTemplate/xsltGetTemplate mechanism
 */
void
xsltFreeTemplateHashes(xsltStylesheetPtr style) {
    if (style->templatesHash != NULL)
	xmlHashFree((xmlHashTablePtr) style->templatesHash,
		    (xmlHashDeallocator) xsltFreeCompMatchList);
    if (style->rootMatch != NULL)
        xsltFreeCompMatchList(style->rootMatch);
    if (style->keyMatch != NULL)
        xsltFreeCompMatchList(style->keyMatch);
    if (style->elemMatch != NULL)
        xsltFreeCompMatchList(style->elemMatch);
    if (style->attrMatch != NULL)
        xsltFreeCompMatchList(style->attrMatch);
    if (style->parentMatch != NULL)
        xsltFreeCompMatchList(style->parentMatch);
    if (style->textMatch != NULL)
        xsltFreeCompMatchList(style->textMatch);
    if (style->piMatch != NULL)
        xsltFreeCompMatchList(style->piMatch);
    if (style->commentMatch != NULL)
        xsltFreeCompMatchList(style->commentMatch);
}

