from HLTrigger.Configuration.common import *

# -- update the WP for the tracker isolation: from 0.07 to 0.09
# -- name of the modules are also updated properly
def customizerForMuonHLT_newTrkIsoWP(process):
    if hasattr(process, "hltMuonTkRelIsolationCut0p07Map"):
        process.hltMuonTkRelIsolationCut0p09Map = process.hltMuonTkRelIsolationCut0p07Map.clone()
        process.hltMuonTkRelIsolationCut0p09Map.CutsPSet.Thresholds = cms.vdouble( 0.09 )
        # replace_with(process.hltMuonTkRelIsolationCut0p07Map, process.hltMuonTkRelIsolationCut0p09Map) # -- it does not change the module name (label)

        insert_modules_after(process, process.hltMuonTkRelIsolationCut0p07Map, process.hltMuonTkRelIsolationCut0p09Map)
        delattr(process, "hltMuonTkRelIsolationCut0p07Map")


    dic_module_before_after = {}
    for module in filters_by_type(process, "HLTMuonIsoFilter"):
        if hasattr(module, "DepTag"):
            # print(module._Labelable__label)

            isFound = False
            _vinputtag = module.DepTag.value()
            for index in range(0, len(_vinputtag)):
                if _vinputtag[index] == "hltMuonTkRelIsolationCut0p07Map":
                    _vinputtag[index] = "hltMuonTkRelIsolationCut0p09Map"
                    isFound = True

            if isFound:
                module_new = module.clone()

                # -- update VInputTag                
                module_new.DepTag = cms.VInputTag( *_vinputtag )

                dic_module_before_after[module] = module_new


    for module in dic_module_before_after.keys():
        module_new = dic_module_before_after[module]

        # -- register on process with a new name
        label_new = module._Labelable__label.replace("0p07", "0p09")
        setattr(process, label_new, module_new)

        insert_modules_after(process, module, module_new)
        delattr(process, module._Labelable__label)

    return process