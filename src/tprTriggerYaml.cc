#include <cpsw_api_builder.h>
#include <cpsw_api_user.h>

#include <tprTriggerYaml.hh>

#include <stdio.h>
#include <string.h>


#define CPSW_TRY_CATCH(X)       try {   \
        (X);                            \
    } catch (CPSWError &e) {            \
        fprintf(stderr,                 \
                "CPSW Error: %s at %s, line %d\n",     \
                e.getInfo().c_str(),    \
                __FILE__, __LINE__);    \
        throw e;                        \
    }


using namespace Tpr;

TprTriggerYaml::TprTriggerYaml(Path core, int bus)
{
     if(!bus) bus_type = _atca;
     else     bus_type = _pcie;

    switch(bus_type) {
        case _atca:
            num_channels = NUM_CHANNELS;
            num_triggers = NUM_TRIGGERS;
            break;
        case _pcie:
            num_channels = PCIE_NUM_CHANNELS;
            num_triggers = PCIE_NUM_TRIGGERS;
            break;
    }



    CPSW_TRY_CATCH(_path               = core);
    CPSW_TRY_CATCH(_path_axiVersion    = _path->findByName("AxiVersion"));


    switch(bus_type) {
        case _atca:
            CPSW_TRY_CATCH(_path_axiSy56040    = _path->findByName("AxiSy56040"));
            CPSW_TRY_CATCH(_path_timingFrameRx = _path->findByName("AmcCarrierTiming/TimingFrameRx"));
            break;
        case _pcie:
            CPSW_TRY_CATCH(_path_axiSy56040    = _path->findByName("SfpXbar"));
            CPSW_TRY_CATCH(_path_timingFrameRx = _path->findByName("TimingCore"));
            break;
    }

    for(int i = 0; i < num_channels; i++) {
        char path_name[128];

        switch(bus_type) {
            case _atca:
                sprintf(path_name, "AmcCarrierTiming/EvrV2CoreTriggers/EvrV2ChannelReg[%d]", i);
                break;
            case _pcie:
                sprintf(path_name, "EvrChannels/Channels[%d]", i);
                break;
        }
        CPSW_TRY_CATCH(_path_evrV2ChnReg[i] = _path->findByName(path_name));
    }

    for(int i = 0; i < num_triggers; i++) {
        char path_name[128];

        switch(bus_type) {
            case _atca:
                sprintf(path_name, "AmcCarrierTiming/EvrV2CoreTriggers/EvrV2TriggerReg[%d]", i);
                break;
            case _pcie:
                sprintf(path_name, "EvrChannels/Triggers[%d]", i);
                break;
        }
        CPSW_TRY_CATCH(_path_evrV2TrgReg[i] = _path->findByName(path_name));
    }


    CPSW_TRY_CATCH(_fpgaVersion    = IScalVal_RO::create(_path_axiVersion->findByName("FpgaVersion")));
    CPSW_TRY_CATCH(_upTimeCnt      = IScalVal_RO::create(_path_axiVersion->findByName("UpTimeCnt")));

    for(int i = 0; i < 4; i++) {
        char path_name[32];
        sprintf(path_name, "OutputConfig[%i]", i);
        CPSW_TRY_CATCH(_outputConfig[i] = IScalVal::create(_path_axiSy56040->findByName(path_name)));
    }

    CPSW_TRY_CATCH(_sofCount      = IScalVal_RO::create(_path_timingFrameRx->findByName("sofCount")));
    CPSW_TRY_CATCH(_eofCount      = IScalVal_RO::create(_path_timingFrameRx->findByName("eofCount")));
    CPSW_TRY_CATCH(_fidCount      = IScalVal_RO::create(_path_timingFrameRx->findByName("FidCount")));
    CPSW_TRY_CATCH(_crcErrCount   = IScalVal_RO::create(_path_timingFrameRx->findByName("CrcErrCount")));
    CPSW_TRY_CATCH(_rxClkCount    = IScalVal_RO::create(_path_timingFrameRx->findByName("RxClkCount")));
    CPSW_TRY_CATCH(_rxRstCount    = IScalVal_RO::create(_path_timingFrameRx->findByName("RxRstCount")));
    CPSW_TRY_CATCH(_rxDecErrCount = IScalVal_RO::create(_path_timingFrameRx->findByName("RxDecErrCount")));
    CPSW_TRY_CATCH(_rxDspErrCount = IScalVal_RO::create(_path_timingFrameRx->findByName("RxDspErrCount")));
    CPSW_TRY_CATCH(_rxCountReset  = IScalVal   ::create(_path_timingFrameRx->findByName("RxCountReset")));
    CPSW_TRY_CATCH(_rxLinkUp      = IScalVal_RO::create(_path_timingFrameRx->findByName("RxLinkUp")));
    CPSW_TRY_CATCH(_rxPolarity    = IScalVal   ::create(_path_timingFrameRx->findByName("RxPolarity")));
    CPSW_TRY_CATCH(_rxReset       = IScalVal   ::create(_path_timingFrameRx->findByName("RxReset")));
    CPSW_TRY_CATCH(_clkSel        = IScalVal   ::create(_path_timingFrameRx->findByName("ClkSel")));
    CPSW_TRY_CATCH(_modeSel       = IScalVal   ::create(_path_timingFrameRx->findByName("ModeSel")));
    CPSW_TRY_CATCH(_modeSelEn     = IScalVal   ::create(_path_timingFrameRx->findByName("ModeSelEn")));
    CPSW_TRY_CATCH(_rxDown        = IScalVal   ::create(_path_timingFrameRx->findByName("RxDown")));
    CPSW_TRY_CATCH(_bypassRst     = IScalVal   ::create(_path_timingFrameRx->findByName("BypassRst")));
    CPSW_TRY_CATCH(_versionErr    = IScalVal_RO::create(_path_timingFrameRx->findByName("VersionErr")));
    CPSW_TRY_CATCH(_msgNoDelay    = IScalVal   ::create(_path_timingFrameRx->findByName("MsgNoDelay")));
    CPSW_TRY_CATCH(_msgDelay      = IScalVal   ::create(_path_timingFrameRx->findByName("MsgDelay")));
    CPSW_TRY_CATCH(_txClkCount    = IScalVal_RO::create(_path_timingFrameRx->findByName("TxClkCount")));
    CPSW_TRY_CATCH(_bypassDoneCount  = IScalVal_RO::create(_path_timingFrameRx->findByName("BypassDoneCount")));
    CPSW_TRY_CATCH(_bypassResetCount = IScalVal_RO::create(_path_timingFrameRx->findByName("BypassResetCount")));
    CPSW_TRY_CATCH(_frameVersion     = IScalVal_RO::create(_path_timingFrameRx->findByName("FrameVersion")));
    
    for(int i = 0; i < num_channels; i++) {
        CPSW_TRY_CATCH(_chnEnable[i]         = IScalVal   ::create(_path_evrV2ChnReg[i]->findByName("Enable")));
        CPSW_TRY_CATCH(_chnBsaEnable[i]      = IScalVal   ::create(_path_evrV2ChnReg[i]->findByName("BsaEnabled")));
        CPSW_TRY_CATCH(_chnDmaEnable[i]      = IScalVal   ::create(_path_evrV2ChnReg[i]->findByName("DmaEnabled")));
        CPSW_TRY_CATCH(_chnRateSelect[i]     = IScalVal   ::create(_path_evrV2ChnReg[i]->findByName("RateSel")));
        CPSW_TRY_CATCH(_chnDestSelect[i]     = IScalVal   ::create(_path_evrV2ChnReg[i]->findByName("DestSel")));
        CPSW_TRY_CATCH(_chnCount[i]          = IScalVal_RO::create(_path_evrV2ChnReg[i]->findByName("Count")));
        CPSW_TRY_CATCH(_chnBsaWindowDelay[i] = IScalVal   ::create(_path_evrV2ChnReg[i]->findByName("BsaWindowDelay")));
        CPSW_TRY_CATCH(_chnBsaWindowSetup[i] = IScalVal   ::create(_path_evrV2ChnReg[i]->findByName("BsaWindowSetup")));
        CPSW_TRY_CATCH(_chnBsaWindowWidth[i] = IScalVal   ::create(_path_evrV2ChnReg[i]->findByName("BsaWindowWidth")));   
    }
    
    for(int i = 0; i < num_triggers; i++) {
        CPSW_TRY_CATCH(_trgEnable[i]           = IScalVal   ::create(_path_evrV2TrgReg[i]->findByName("Enable")));
        CPSW_TRY_CATCH(_trgSourceMask[i]       = IScalVal   ::create(_path_evrV2TrgReg[i]->findByName("Source")));
        CPSW_TRY_CATCH(_trgPolarity[i]         = IScalVal   ::create(_path_evrV2TrgReg[i]->findByName("Polarity")));
        CPSW_TRY_CATCH(_trgDelay[i]            = IScalVal   ::create(_path_evrV2TrgReg[i]->findByName("Delay")));
        CPSW_TRY_CATCH(_trgWidth[i]            = IScalVal   ::create(_path_evrV2TrgReg[i]->findByName("Width")));
        CPSW_TRY_CATCH(_trgDelayTap[i]         = IScalVal   ::create(_path_evrV2TrgReg[i]->findByName("DelayTap")));
        CPSW_TRY_CATCH(_trgDelayTapReadback[i] = IScalVal_RO::create(_path_evrV2TrgReg[i]->findByName("DelayTapReadback")));
        CPSW_TRY_CATCH(_trgComplEnable[i]      = IScalVal   ::create(_path_evrV2TrgReg[i]->findByName("ComplEn")));
        CPSW_TRY_CATCH(_trgComplAnd[i]         = IScalVal   ::create(_path_evrV2TrgReg[i]->findByName("ComplAnd")));
    }


}


void TprTriggerYaml::InitLCLS1Mode(void)
{
    uint32_t zero(0), dontcare(0x20000);
    CPSW_TRY_CATCH(_clkSel->setVal(&zero));         /* Select LCLS1 mode clock */
    
    for(uint32_t i = 0; i < num_channels; i++) {
        CPSW_TRY_CATCH(_chnDestSelect[i]->setVal(&dontcare));  /* destination, don't care */
        CPSW_TRY_CATCH(_trgSourceMask[i]->setVal(&i));         /* one to one mapping between channel and trigger */
    }
}

void TprTriggerYaml::InitLCLS2Mode(void)
{
    uint32_t one(1);
    CPSW_TRY_CATCH(_clkSel->setVal(&one));      /* Select LCLS2 mode clock */
    
    
}

void TprTriggerYaml::SetClkSel(uint32_t clock_mode)
{
    uint32_t zero(0),one(1);
    CPSW_TRY_CATCH(_clkSel->setVal(clock_mode? &one:&zero));
    
    if(_debug_) printf("TprTriggerYaml (%p): set cls_sel (%8.8x)\n", this, clock_mode?one:zero);
}

void TprTriggerYaml::SetModeSel(uint32_t mode)
{
    uint32_t zero(0), one(1);
    CPSW_TRY_CATCH(_modeSel->setVal(mode? &one:&zero));

    if(_debug_) printf("TprTriggerYaml (%p): set mode_sel (%8.8x)\n", this, mode?one:zero);
}

void TprTriggerYaml::SetModeSelEn(uint32_t enable)
{
    uint32_t zero(0), one(1);
    CPSW_TRY_CATCH(_modeSelEn->setVal(enable? &one:&zero));

    if(_debug_) printf("TprTriggerYaml (%p): set mode_sel_en (%8.8x)\n", this, enable?one:zero);
}


void TprTriggerYaml::SetMsgDelay(uint32_t delay_ticks)
{
    CPSW_TRY_CATCH(_msgDelay->setVal(&delay_ticks));
    if(_debug_) printf("TprTriggerYaml (%p): set msg delay (%8.8lx) %lu\n", this, (unsigned long)delay_ticks, (unsigned long)delay_ticks);
}

void TprTriggerYaml::ChannelEnable(int channel, uint32_t enable)
{
    if(channel >= num_channels) return;

    uint32_t zero(0), one(1);
   
    CPSW_TRY_CATCH(_chnEnable[channel]->setVal(enable?&one:&zero));
    
    if(_debug_) printf("TprTriggerYaml (%p): set channel enable (chn %x, enable %x)\n", this, channel, enable?one:zero); 
    
}

void TprTriggerYaml::SetEventCode(int channel, uint32_t event)
{
    if(channel >= num_channels) return;

    uint32_t  _dontcare(0x20000);
    CPSW_TRY_CATCH(_chnDestSelect[channel]->setVal(&_dontcare));
    uint32_t _event = (0x00000002<<11) | (0x000003ff & event);
    CPSW_TRY_CATCH(_chnRateSelect[channel]->setVal(&_event));
    
    if(_debug_) {
        printf("TprTriggerYaml (%p): destination select (chn %x, dest sel %8.8x)\n", this, channel, _dontcare);
        printf("TprTriggerYaml (%p): rate select(event code) (chn %x, event code %8.8x)\n", this, channel, _event);
    }
}

void TprTriggerYaml::SetFixedRate(int channel, uint32_t rate)
{
    if(channel >= num_channels) return;

    uint32_t _rate = (0x000000<<11) | (0x0000000f & rate);
    CPSW_TRY_CATCH(_chnRateSelect[channel]->setVal(&_rate));
    
    if(_debug_) printf("TprTriggerYaml (%p): rate select (fixed rate) (chn %x, rate select %8.8x)\n", this, channel, _rate);
}

void TprTriggerYaml::SetACRate(int channel, uint32_t ts_mask, uint32_t rate)
{
    if(channel >= num_channels) return;

    uint32_t _rate = (0x00000001<<11) | (0x0000003f & ts_mask)<< 3 | (0x00000007 & rate);
    CPSW_TRY_CATCH(_chnRateSelect[channel]->setVal(&_rate));
    
    if(_debug_) printf("TprTriggerYaml (%p): rate select (AC rate) (chn %x, rate select %8.8x)\n", this, channel, _rate);
}

void TprTriggerYaml::SetSeqBit(int channel, uint32_t seq_bit)
{
    if(channel >= num_channels) return;

    uint32_t _seq_bit = (0x00000002<<11) | (0x000003ff & seq_bit);
    CPSW_TRY_CATCH(_chnRateSelect[channel]->setVal(&_seq_bit));
    
    if(_debug_) printf("TprTriggerYaml (%p): rate select (Seq Bit) (chn %x, rate select %8.8x)\n", this, channel, _seq_bit);
}

void TprTriggerYaml::SetInclusionMask(int channel, uint32_t dest_mask)
{
    if(channel >= num_channels) return;

    uint32_t _mask = (0x00000000<<16) | (0x0000ffff & dest_mask);
    CPSW_TRY_CATCH(_chnDestSelect[channel]->setVal(&_mask));
    
    if(_debug_) printf("TprTriggerYaml (%p): dest select (inclusion) (chn %x, dest select %8.8x)\n", this, channel, _mask);
}

void TprTriggerYaml::SetExclusionMask(int channel, uint32_t dest_mask)
{
    if(channel >= num_channels) return;

    uint32_t _mask = (0x00000001<<16) | (0x0000ffff & dest_mask);
    CPSW_TRY_CATCH(_chnDestSelect[channel]->setVal(&_mask));
    
    if(_debug_) printf("TprTriggerYaml (%p): dest select (excusion) (chn %x, dest select %8.8x)\n", this, channel, _mask);
}

void TprTriggerYaml::SetDontCareMask(int channel)
{
    if(channel >= num_channels) return;

    uint32_t _mask = (0x00000002<<16);
    CPSW_TRY_CATCH(_chnDestSelect[channel]->setVal(&_mask));
    
    if(_debug_) printf("TprTriggerYaml (%p): dest select (dont care) (chn %x, dest select %8.8x)\n", this, channel, _mask);
}

void TprTriggerYaml::TriggerEnable(int trigger, uint32_t enable)
{
    if(trigger >= num_triggers) return;

    uint32_t zero(0), one(1);
    
    CPSW_TRY_CATCH(_trgEnable[trigger]->setVal(enable?&one:&zero));
    
    if(_debug_) printf("TprTriggerYaml (%p): trigger enable (trg %x, enable %x)\n", this, trigger, enable?one:zero);
}

void TprTriggerYaml::SetSourceMask(int trigger, uint32_t source)
{
    if(trigger >= num_triggers) return;

    //uint32_t _source = 0x00000001 << source;
    uint32_t _source = 0x0000000f & source;
    
    CPSW_TRY_CATCH(_trgSourceMask[trigger]->setVal(&_source));
    
    if(_debug_) printf("TprTriggerYaml (%p): source (trg %x, source %x)\n", this, trigger, _source);
}

void TprTriggerYaml::SetPolarity(int trigger, uint32_t polarity)
{
    if(trigger >= num_triggers) return;

    uint32_t falling(0), rising(1);
    
    CPSW_TRY_CATCH(_trgPolarity[trigger]->setVal(!polarity?&falling:&rising));
    
    if(_debug_) printf("TprTriggerYaml (%p): polarity (trg %x, prolarity %x)\n", this, trigger, !polarity?falling:rising);
}

void TprTriggerYaml::SetDelay(int trigger, uint32_t delay_ticks)
{
    if(trigger >= num_triggers) return;

    CPSW_TRY_CATCH(_trgDelay[trigger]->setVal(&delay_ticks));
    
    if(_debug_) printf("TprTriggerYaml (%p): delay (trg %x, delay %8.8lx) %lu\n", this, trigger, (unsigned long)delay_ticks, (unsigned long)delay_ticks);
}

void TprTriggerYaml::SetWidth(int trigger, uint32_t width_ticks)
{
    if(trigger >= num_triggers) return;

    CPSW_TRY_CATCH(_trgWidth[trigger]->setVal(&width_ticks));
    
    if(_debug_) printf("TprTriggerYaml (%p): width (trg %x, width %8.8lx) %lu\n", this, trigger, (unsigned long)width_ticks, (unsigned long)width_ticks);
}  

void TprTriggerYaml::SetComplTrg(int trigger, uint32_t comp)
{
    if(trigger >= num_triggers) return;

    uint32_t disable(0), enable(1), compl_or(0), compl_and(1);
    _compltrg comm = (_compltrg) comp;

    switch(comm) {
        case _disable:
            CPSW_TRY_CATCH(_trgComplEnable[trigger]->setVal(&disable));
            break;
        case _or:
            CPSW_TRY_CATCH(_trgComplEnable[trigger]->setVal(&enable));
            CPSW_TRY_CATCH(_trgComplAnd[trigger]->setVal(&compl_or));
            break;
        case _and:
            CPSW_TRY_CATCH(_trgComplEnable[trigger]->setVal(&enable));
            CPSW_TRY_CATCH(_trgComplAnd[trigger]->setVal(&compl_and));
            break;
        default:
            break;
    }

    if(_debug_) printf("TprTriggerYaml (%d): compl_trigger(trg %x, comm %8.8lx)\n", this, trigger, (unsigned long) comp);
}

void TprTriggerYaml::SetDelayTap(int trigger, uint32_t tap)
{
    if(trigger >= num_triggers) return;

    if(tap > 62) tap = 62;
    CPSW_TRY_CATCH(_trgDelayTap[trigger]->setVal(&tap));
}

void TprTriggerYaml::report(void)
{
    char *str_bus_type;
    switch(bus_type) {
        case _atca:
            str_bus_type = "ATCA TPR";
            break;
        case _pcie:
            str_bus_type = "PCIe TPR";
            break;
    }
    printf("\t%s\n", str_bus_type);
    printf("\tFPGA version: %8.8x\n", fpgaVersion()); 
    printf("\tUpTime count: %8.8x\n", upTimeCount());
    printf("\tStart of Frame count: %8.8x\n", sofCount());
    printf("\tEnd of Frame count:   %8.8x\n", eofCount());
    printf("\tFiducial count:       %8.8x\n", fidCount());
    printf("\tCRC Error count:      %8.8x\n", crcErrCount());
    printf("\tRX Clock count:       %8.8x\n", rxClkCount());
    printf("\tRX Link Reset count:  %8.8x\n", rxRstCount());
    printf("\tRX 8b/10b Decode Error count: %8.8x\n", rxDecErrCount());
    printf("\tRX Disparity Error count:     %8.8x\n", rxDspErrCount());
    printf("\tRX Link Up status:    %s\n", rxLinkStatus()?"Up":"Down");
    printf("\tTX Clock count:       %8.8x\n", txClkCount());
    printf("\tFrame Version Error:  %s\n", versionErr()?"Error":"Good");
    printf("\tFrame Version:        %8.8x\n", frameVersion());
    
    for(int i = 0; i < num_channels; i++) {
        printf("\tChannel [%x]\n", i);
        printf("\t\tEnable:             %8.8x\n", channelEnable(i));
        printf("\t\tRate Select:        %8.8x\n", channelRateSelect(i));
        printf("\t\tDestination Select: %8.8x\n", channelDestSelect(i));
        printf("\t\tChannel Count:      %8.8x\n", channelCount(i));  
    }
    
    for(int i =0; i < num_triggers; i++) {
        printf("\tTrigger [%x]\n", i);
        printf("\t\tEnable:             %8.8x\n", triggerEnable(i));
        printf("\t\tSource Mask:        %8.8x\n", triggerSourceMask(i));
        printf("\t\tPolarity:           %8.8x\n", triggerPolarity(i));
        printf("\t\tWidth:              %8.8x\n", triggerWidth(i));
        printf("\t\tDelay:              %8.8x\n", triggerDelay(i));
    }
    
}

uint32_t TprTriggerYaml::channelEnable(int channel)
{
    if(channel >= num_channels)  return -1;

    uint32_t        val;
    IndexRange      rng(0);
    
    CPSW_TRY_CATCH(_chnEnable[channel]->getVal(&val, 1,  &rng));
    
    return val;
}

uint32_t TprTriggerYaml::channelRateSelect(int channel)
{
    if(channel >= num_channels)  return -1;

    uint32_t        val;
    IndexRange      rng(0);
    
    CPSW_TRY_CATCH(_chnRateSelect[channel]->getVal(&val, 1, &rng));
    
    return val;
}

uint32_t TprTriggerYaml::channelDestSelect(int channel)
{
    if(channel >= num_channels)  return -1;

    uint32_t        val;
    IndexRange      rng(0);
    
    CPSW_TRY_CATCH(_chnDestSelect[channel]->getVal(&val, 1, &rng));
    
    return val;
}

uint32_t TprTriggerYaml::channelCount(int channel)
{
    if(channel >= num_channels)  return -1;

    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_chnCount[channel]->getVal(&val, 1, &rng));
    
    return val;
}

uint32_t TprTriggerYaml::triggerEnable(int trigger)
{
    if(trigger >= num_triggers) return -1;

    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_trgEnable[trigger]->getVal(&val, 1, &rng));
    
    return val;
}

uint32_t TprTriggerYaml::triggerSourceMask(int trigger)
{
    if(trigger >= num_triggers) return -1;

    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_trgSourceMask[trigger]->getVal(&val, 1, &rng));
    
    return val;
}

uint32_t TprTriggerYaml::triggerPolarity(int trigger)
{
    if(trigger >= num_triggers) return -1;

    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_trgPolarity[trigger]->getVal(&val, 1, &rng));
    
    return val;

}

uint32_t TprTriggerYaml::triggerWidth(int trigger)
{
    if(trigger >= num_triggers) return -1;

    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_trgWidth[trigger]->getVal(&val, 1, &rng));
    
    return val;
}

uint32_t TprTriggerYaml::triggerDelay(int trigger)
{
    if(trigger >= num_triggers) return -1;

    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_trgDelay[trigger]->getVal(&val, 1, &rng));
    
    return val;
}



uint32_t TprTriggerYaml::fpgaVersion(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_fpgaVersion->getVal(&val,1,&rng));
    
    return val;
}

uint32_t TprTriggerYaml::upTimeCount(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    _upTimeCnt->getVal(&val,1,&rng);
    
    return val;
}


uint32_t TprTriggerYaml::sofCount(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_sofCount->getVal(&val,1,&rng));
    
    return val;
}

uint32_t TprTriggerYaml::eofCount(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_eofCount->getVal(&val,1,&rng));
    
    return val;
}

uint32_t TprTriggerYaml::fidCount(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_fidCount->getVal(&val,1,&rng));
    
    return val;
}

uint32_t TprTriggerYaml::crcErrCount(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_crcErrCount->getVal(&val,1,&rng));
    
    return val;
}

uint32_t TprTriggerYaml::rxClkCount(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    
    CPSW_TRY_CATCH(_rxClkCount->getVal(&val,1,&rng));
    
    return val;
}

uint32_t TprTriggerYaml::txClkCount(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    
    CPSW_TRY_CATCH(_txClkCount->getVal(&val,1,&rng));
    
    return val;
}


uint32_t TprTriggerYaml::rxRstCount(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_rxRstCount->getVal(&val,1,&rng));
    
    return val;
}

uint32_t TprTriggerYaml::rxDecErrCount(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_rxDecErrCount->getVal(&val,1,&rng));
    
    return val;
}

uint32_t TprTriggerYaml::rxDspErrCount(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_rxDspErrCount->getVal(&val,1,&rng));
    
    return val;
}

uint32_t TprTriggerYaml::rxLinkStatus(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_rxLinkUp->getVal(&val,1,&rng));
    
    return val;
}

uint32_t TprTriggerYaml::versionErr(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_versionErr->getVal(&val,1,&rng));
    
    return val;
}


uint32_t TprTriggerYaml::frameVersion(void)
{
    uint32_t      val;
    IndexRange    rng(0);
    
    CPSW_TRY_CATCH(_frameVersion->getVal(&val,1,&rng));
    
    return val;
}
