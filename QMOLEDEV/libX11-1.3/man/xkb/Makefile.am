# Have to fake out automake by calling our man pages "DATA" since "MANS" won't
# accept .3x as a valid man page suffix

libmandir = $(LIB_MAN_DIR)

LIB_MAN_DIR_SUFFIX = $(LIB_MAN_DIR:@mandir@/man%=%)

libman_PRE = \
	XkbActionCtrls.man                      \
	XkbAddDeviceLedInfo.man                 \
	XkbAddGeomColor.man                     \
	XkbAddGeomDoodad.man                    \
	XkbAddGeomKey.man                       \
	XkbAddGeomKeyAlias.man                  \
	XkbAddGeomOutline.man                   \
	XkbAddGeomOverlay.man                   \
	XkbAddGeomOverlayKey.man                \
	XkbAddGeomOverlayRow.man                \
	XkbAddGeomProperty.man                  \
	XkbAddGeomRow.man                       \
	XkbAddGeomSection.man                   \
	XkbAddGeomShape.man                     \
	XkbAddSymInterpret.man                  \
	XkbAllocClientMap.man                   \
	XkbAllocCompatMap.man                   \
	XkbAllocControls.man                    \
	XkbAllocDeviceInfo.man                  \
	XkbAllocDeviceLedInfo.man               \
	XkbAllocGeomColors.man                  \
	XkbAllocGeomDoodads.man                 \
	XkbAllocGeomKeyAliases.man              \
	XkbAllocGeomKeys.man                    \
	XkbAllocGeomOutlines.man                \
	XkbAllocGeomOverlayKey.man              \
	XkbAllocGeomOverlayRows.man             \
	XkbAllocGeomOverlays.man                \
	XkbAllocGeomPoints.man                  \
	XkbAllocGeomProps.man                   \
	XkbAllocGeomRows.man                    \
	XkbAllocGeomSectionDoodads.man          \
	XkbAllocGeomSections.man                \
	XkbAllocGeomShapes.man                  \
	XkbAllocGeometry.man                    \
	XkbAllocIndicatorMaps.man               \
	XkbAllocKeyboard.man                    \
	XkbAllocNames.man                       \
	XkbAllocServerMap.man                   \
	XkbApplyCompatMapToKey.man              \
	XkbBell.man                             \
	XkbBellEvent.man                        \
	XkbChangeControls.man                   \
	XkbChangeDeviceInfo.man                 \
	XkbChangeEnabledControls.man            \
	XkbChangeIndicators.man                 \
	XkbChangeMap.man                        \
	XkbChangeNames.man                      \
	XkbChangeTypesOfKey.man                 \
	XkbComputeRowBounds.man                 \
	XkbComputeSectionBounds.man             \
	XkbComputeShapeBounds.man               \
	XkbComputeShapeTop.man                  \
	XkbCopyKeyType.man                      \
	XkbCopyKeyTypes.man                     \
	XkbDeviceBell.man                       \
	XkbDeviceBellEvent.man                  \
	XkbFindOverlayForKey.man                \
	XkbForceBell.man                        \
	XkbForceDeviceBell.man                  \
	XkbFreeClientMap.man                    \
	XkbFreeCompatMap.man                    \
	XkbFreeComponentList.man                \
	XkbFreeControls.man                     \
	XkbFreeDeviceInfo.man                   \
	XkbFreeGeomColors.man                   \
	XkbFreeGeomDoodads.man                  \
	XkbFreeGeomKeyAliases.man               \
	XkbFreeGeomKeys.man                     \
	XkbFreeGeomOutlines.man                 \
	XkbFreeGeomOverlayKeys.man              \
	XkbFreeGeomOverlayRows.man              \
	XkbFreeGeomOverlays.man                 \
	XkbFreeGeomPoints.man                   \
	XkbFreeGeomProperties.man               \
	XkbFreeGeomRows.man                     \
	XkbFreeGeomSections.man                 \
	XkbFreeGeomShapes.man                   \
	XkbFreeGeometry.man                     \
	XkbFreeIndicatorMaps.man                \
	XkbFreeKeyboard.man                     \
	XkbFreeNames.man                        \
	XkbFreeServerMap.man                    \
	XkbGetAccessXTimeout.man                \
	XkbGetAutoRepeatRate.man                \
	XkbGetAutoResetControls.man             \
	XkbGetBounceKeysDelay.man               \
	XkbGetCompatMap.man                     \
	XkbGetControls.man                      \
	XkbGetControlsChanges.man               \
	XkbGetDetectableAutorepeat.man          \
	XkbGetDeviceButtonActions.man           \
	XkbGetDeviceInfo.man                    \
	XkbGetDeviceInfoChanges.man             \
	XkbGetDeviceLedInfo.man                 \
	XkbGetGeometry.man                      \
	XkbGetIndicatorChanges.man              \
	XkbGetIndicatorMap.man                  \
	XkbGetIndicatorState.man                \
	XkbGetKeyActions.man                    \
	XkbGetKeyBehaviors.man                  \
	XkbGetKeyExplicitComponents.man         \
	XkbGetKeyModifierMap.man                \
	XkbGetKeySyms.man                       \
	XkbGetKeyTypes.man                      \
	XkbGetKeyVirtualModMap.man              \
	XkbGetKeyboard.man                      \
	XkbGetKeyboardByName.man                \
	XkbGetMap.man                           \
	XkbGetNameChanges.man                   \
	XkbGetNamedGeometry.man                 \
	XkbGetNamedIndicator.man                \
	XkbGetNames.man                         \
	XkbGetSlowKeysDelay.man                 \
	XkbGetState.man                         \
	XkbGetStickyKeysOptions.man             \
	XkbGetUpdatedMap.man                    \
	XkbGetVirtualMods.man                   \
	XkbGetXlibControls.man                  \
	XkbIgnoreExtension.man                  \
	XkbInitCanonicalKeyTypes.man            \
	XkbKeyAction.man                        \
	XkbKeyActionEntry.man                   \
	XkbKeyActionsPtr.man                    \
	XkbKeyGroupInfo.man                     \
	XkbKeyGroupWidth.man                    \
	XkbKeyGroupsWidth.man                   \
	XkbKeyHasActions.man                    \
	XkbKeyNumActions.man                    \
	XkbKeyNumGroups.man                     \
	XkbKeyNumSyms.man                       \
	XkbKeySymEntry.man                      \
	XkbKeySymsOffset.man                    \
	XkbKeySymsPtr.man                       \
	XkbKeyType.man                          \
	XkbKeyTypeIndex.man                     \
	XkbKeyTypesForCoreSymbols.man           \
	XkbKeycodeToKeysym.man                  \
	XkbKeysymToModifiers.man                \
	XkbLatchGroup.man                       \
	XkbLatchModifiers.man                   \
	XkbLibraryVersion.man                   \
	XkbListComponents.man                   \
	XkbLockGroup.man                        \
	XkbLockModifiers.man                    \
	XkbLookupKeyBinding.man                 \
	XkbLookupKeySym.man                     \
	XkbModActionVMods.man                   \
	XkbNoteControlsChanges.man              \
	XkbNoteDeviceChanges.man                \
	XkbNoteIndicatorChanges.man             \
	XkbNoteNameChanges.man                  \
	XkbOpenDisplay.man                      \
	XkbOutOfRangeGroupInfo.man              \
	XkbOutOfRangeGroupNumber.man            \
	XkbPtrActionX.man                       \
	XkbPtrActionY.man                       \
	XkbQueryExtension.man                   \
	XkbRefreshKeyboardMapping.man           \
	XkbResizeDeviceButtonActions.man        \
	XkbResizeKeyActions.man                 \
	XkbResizeKeySyms.man                    \
	XkbResizeKeyType.man                    \
	XkbSAActionSetCtrls.man                 \
	XkbSAGroup.man                          \
	XkbSAPtrDfltValue.man                   \
	XkbSARedirectSetVMods.man               \
	XkbSARedirectSetVModsMask.man           \
	XkbSARedirectVMods.man                  \
	XkbSARedirectVModsMask.man              \
	XkbSAScreen.man                         \
	XkbSASetGroup.man                       \
	XkbSASetPtrDfltValue.man                \
	XkbSASetScreen.man                      \
	XkbSelectEventDetails.man               \
	XkbSelectEvents.man                     \
	XkbSetAccessXTimeout.man                \
	XkbSetAutoRepeatRate.man                \
	XkbSetAutoResetControls.man             \
	XkbSetBounceKeysDelay.man               \
	XkbSetCompatMap.man                     \
	XkbSetControls.man                      \
	XkbSetDebuggingFlags.man                \
	XkbSetDetectableAutorepeat.man          \
	XkbSetDeviceButtonActions.man           \
	XkbSetDeviceInfo.man                    \
	XkbSetIgnoreLockMods.man                \
	XkbSetIndicatorMap.man                  \
	XkbSetMap.man                           \
	XkbSetModActionVMods.man                \
	XkbSetNamedIndicator.man                \
	XkbSetNames.man                         \
	XkbSetPtrActionX.man                    \
	XkbSetPtrActionY.man                    \
	XkbSetServerInternalMods.man            \
	XkbSetXlibControls.man                  \
	XkbTranslateKeyCode.man                 \
	XkbTranslateKeySym.man                  \
	XkbUpdateMapFromCore.man                \
	XkbVirtualModsToReal.man                

# What we actually install 

if MANPAGES
libman_DATA = $(libman_PRE:man=@LIB_MAN_SUFFIX@)
endif

EXTRA_DIST = $(libman_PRE)

CLEANFILES = $(libman_DATA)

include $(top_srcdir)/cpprules.in

SUFFIXES += .$(LIB_MAN_SUFFIX) .man

XORGRELSTRING = @PACKAGE_STRING@
  XORGMANNAME = X Version 11

MANDEFS = -D__libmansuffix__=$(LIB_MAN_SUFFIX) \
	-D__xorgversion__="\"$(XORGRELSTRING)\" \"$(XORGMANNAME)\""

CPP_MAN_FLAGS = $(MANDEFS) $(EXTRAMANDEFS)

.man.$(LIB_MAN_SUFFIX):
	$(cpp_verbose)$(RAWCPP) $(RAWCPPFLAGS) $(CPP_MAN_FLAGS) < $< | $(CPP_SED_MAGIC) > $@

