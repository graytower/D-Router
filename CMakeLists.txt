cmake_minimum_required(VERSION 3.16)
project(NDN_NEW)

set(CMAKE_CXX_STANDARD 14)

#MATH(EXPR stack_size "1024000000")
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")
#set(ulimit -s 81920)
#set_target_properties(<target> PROPERTIES LINK_FLAGS -Wl,--stack,10000000)

add_executable(NDN_NEW main.cpp Link.cpp Link.h  Interest.cpp Interest.h Router.cpp Router.h Port.cpp Port.h
        Network.cpp Network.h Topology.cpp Topology.h DeviceName.h NameLSA.cpp
        NameLSA.h NamePrefix.cpp NamePrefix.h CS.cpp CS.h CSRecord.h CSRecord.cpp Host.cpp Host.h Data.cpp Data.h LSDB.cpp LSDB.h
        PIT.cpp PIT.h PITRecord.cpp PITRecord.h FIB.cpp FIB.h SyncInterest.cpp SyncInterest.h
        SyncData.cpp SyncData.h RemoteServer.cpp RemoteServer.h
        WaitingQueItem.cpp WaitingQueItem.h ConfigFileInStream.cpp ConfigFileInStream.h CSFinder.h
        SentInterestTable.cpp SentInterestTable.h SentInterestRecord.cpp SentInterestRecord.h Logger.cpp Logger.h
        LoggerRecord.cpp LoggerRecord.h IndexTable.cpp IndexTable.h Helper.cpp Helper.h typedef.h typedef.cpp
        TimeLogMap.cpp TimeLogMap.h TimePair.cpp TimePair.h TimeItem.cpp TimeItem.h RawJobsTime.cpp RawJobsTime.h  TimeQue.cpp TimeQue.h Pkt.cpp Pkt.h Recorder.cpp Recorder.h Recorder.h)

find_package(Threads REQUIRED)
target_link_libraries(NDN_NEW Threads::Threads)