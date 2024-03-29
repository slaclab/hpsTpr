//////////////////////////////////////////////////////////////////////////////
// This file is part of 'hpsTpr'.
// It is subject to the license terms in the LICENSE.txt file found in the 
// top-level directory of this distribution and at: 
//    https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html. 
// No part of 'hpsTpr', including this file, 
// may be copied, modified, propagated, or distributed except according to 
// the terms contained in the LICENSE.txt file.
//////////////////////////////////////////////////////////////////////////////
#ifndef TPR_PATTERN_YAML_hh
#define TPR_PATTERN_YAML_hh

#include <cpsw_api_builder.h>

#include <stdint.h>
#include <vector>


namespace Tpr {

#pragma pack (push)
#pragma pack (1)
    class TprStream {
        public:
        uint32_t pulse_id;
        uint32_t event_code[8];
        uint16_t dtype;
        uint16_t version;
        uint32_t dmod[6];
        uint64_t epics_time;
        uint32_t edef_avg_done;
        uint32_t edef_minor;
        uint32_t edef_major;
        uint32_t edef_init;
    };
#pragma pack (pop)

#define TPR_STREAM_SIZE   sizeof(Tpr::TprStream)
    
    class TprPatternYaml {
    
        private:
        
            Stream _stream;
            
        public:
            TprStream  *_p_stream_buf;
            uint32_t   _stream_size;
            uint32_t   _read_cnt;
            
            
        protected:
            Path          _path;
            ScalVal_RO    _serverRemoteIp;
            ScalVal_RO    _serverRemotePort;
            ScalVal       _msgNoDelay;
            ScalVal       _outputConfig0;
            ScalVal       _outputConfig1;
            ScalVal       _outputConfig2;
            ScalVal       _outputConfig3;
            

        public:
            int _debug_;
            TprPatternYaml(Path core, Path tstream);
            
            uint32_t GetDestinationIp(void);
            uint32_t GetDestinationPort(void);
            uint32_t GetOutputConfig0(void);
            uint32_t GetOutputConfig1(void);
            uint32_t GetOutputConfig2(void);
            uint32_t GetOutputConfig3(void);
            
            void     SetupTimingFrameInput(uint32_t input);
            void     SetupMsgNoDelay(uint32_t no_delay);
            
            void     TrainingStream(void);
            void     Read(TprStream *buf, int stream_length);
            int PrintPattern(TprStream *p, uint32_t size); 

    };

};


#endif /* TPR_PATTERN_YAML_hh */
