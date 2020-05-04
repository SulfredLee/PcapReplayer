#ifndef COMMON_H
#define COMMON_H

enum class PlayerStatus{
    Stop,
    Pause,
    Play
};

enum class PlayerMsg{
    Stop,
    Pause,
    Play
};

const unsigned int PCAPGOBLEHEADERSIZE = 24;
const unsigned int PCAPLOCALHEADERSIZE = 16;
const unsigned int DATAGRAME_DATA_OFFSET = 42;
#endif
