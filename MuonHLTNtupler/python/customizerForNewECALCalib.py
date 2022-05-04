import FWCore.ParameterSet.Config as cms

# -- ref: https://indico.cern.ch/event/1154993/contributions/4849535/attachments/2437334/4174572/TSG_Weekly_04May22.pdf
def customizerForMuonHLT_NewECALCalib(process):
    process.GlobalTag.toGet = cms.VPSet(

        cms.PSet(
            record = cms.string("EcalLaserAlphasRcd"),
            tag = cms.string("EcalLaserAlphas_UL_Run1_Run2_2018_lastIOV_movedTo1"),
            connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
        ),

        cms.PSet(
            record = cms.string("EcalIntercalibConstantsRcd"),
            tag = cms.string("EcalIntercalibConstants_UL_Run1_Run2_2018_lastIOV_movedTo1"),
            connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
        ),
     )

    return process