import FWCore.ParameterSet.Config as cms

def customizerForMuonHLT_NewHCALIsoWP(process, wp = "0.985"):
    # -- singal efficiency: 98.5%
    newWP_barrel = 0;
    newWP_endcap = 0;
    if wp == "0.985":
        newWP_barrel = 0.177
        newWP_endcap = 0.240
    elif wp == "0.99":
        newWP_barrel = 0.203
        newWP_endcap = 0.270
    else:
        print("wrong WP")
        return

    for module in filters_by_type(process, "HLTMuonGenericFilter"):
        if module.varTag.value() == "hltMuonHcalRegPFClusterIsoForMuons" # -- HCAL isolation
        module.thrOverEEB = cms.vdouble( newWP_barrel ) # -- new WP
        module.thrOverEEE = cms.vdouble( newWP_endcap ) # -- new WP



