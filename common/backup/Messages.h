#pragma once

#define SENSOR_LENGTH 5
#define OPERATOR_NAME_LENGTH 100


// extend it on demand
typedef enum _btsom_msg_type_t {
    NO_MSG,
    INIT_MAMBA_REQ,
    INIT_MAMBA_RESP,
    INIT_TELESCOPE_REQ,
    INIT_TELESCOPE_RESP,
    START_EVENT_MAMBA_REQ,
    START_EVENT_MAMBA_RESP,
    START_EVENT_TIMEPIX_REQ,
    START_EVENT_TIMEPIX_RESP,
    STOP_EVENT_MAMBA_REQ,
    STOP_EVENT_MAMBA_RESP,
    STOP_EVENT_TIMEPIX_REQ,
    STOP_EVENT_TIMEPIX_RESP
} MSG_TYPE;

typedef struct msg_hdr_t {
    MSG_TYPE MsgType;
    long seqNum;
} __attribute__((packed))MSG_HDR;


typedef enum _runType
{
    CUSTOM=0,
    BIAS,
    ANGLE
} __attribute__((packed)) RunType;

typedef struct init_event_t{
    RunType runType;
    int dutRunNumber;
    int biasVoltage;
    int angle;
    int stageX;
    int stageY;
    double temperature;
    unsigned short sector;
    char sensorName[SENSOR_LENGTH];
    char operatorName[SENSOR_LENGTH];
}__attribute__((packed))InitEventReqStruct;


typedef struct init_event_rsp{
    int keplerRunNumber;
}__attribute__((packed))InitEventRespStruct;

typedef struct stop_event_req{
    bool isGood;
}__attribute__((packed))StopEventReqStruct;


typedef union msg_data_t {
    bool empty;
    InitEventReqStruct initEventReq;
    InitEventRespStruct initEventResp;
    StopEventReqStruct stopEventReq;
} __attribute__((packed))MSG_DATA;
