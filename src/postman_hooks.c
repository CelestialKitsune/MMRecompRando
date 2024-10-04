#include "modding.h"
#include "global.h"
#include "z64msgevent.h"

#include "apcommon.h"

struct EnPm;

typedef void (*EnPmActionFunc)(struct EnPm*, PlayState*);
typedef s32 (*EnPmFunc2)(struct EnPm*, PlayState*);

#define ENPM_GET_PATH_INDEX(thisx) ((thisx)->params & 0xFF)

typedef struct EnPm {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnPmActionFunc actionFunc;
    /* 0x18C */ EnPmFunc2 unk_18C;
    /* 0x190 */ ColliderCylinder colliderCylinder;
    /* 0x1DC */ ColliderSphere colliderSphere;
    /* 0x234 */ Path* timePath;
    /* 0x238 */ Vec3f timePathTargetPos;
    /* 0x244 */ f32 timePathProgress;
    /* 0x248 */ s32 timePathTotalTime;
    /* 0x24C */ s32 timePathWaypointTime;
    /* 0x250 */ s32 timePathWaypoint;
    /* 0x254 */ s32 timePathElapsedTime;
    /* 0x258 */ u8 scheduleResult;
    /* 0x25C */ MsgScript* msgScript;
    /* 0x260 */ s8 unk_260;
    /* 0x264 */ s32 msgScriptPos;
    /* 0x268 */ Actor* unk_268;
    /* 0x26C */ Vec3f unk_26C;
    /* 0x278 */ Vec3f unk_278;
    /* 0x284 */ Vec3f unk_284;
    /* 0x290 */ Vec3s unk_290;
    /* 0x296 */ Vec3s jointTable[16];
    /* 0x2F6 */ Vec3s morphTable[16];
    /* 0x356 */ u16 unk_356;
    /* 0x358 */ u16 unk_358;
    /* 0x35C */ f32 unk_35C;
    /* 0x360 */ f32 unk_360;
    /* 0x364 */ f32 unk_364;
    /* 0x368 */ f32 unk_368;
    /* 0x36C */ s16 unk_36C;
    /* 0x36E */ s16 unk_36E;
    /* 0x370 */ s16 unk_370;
    /* 0x372 */ s16 unk_372;
    /* 0x374 */ s16 timePathTimeSpeed;
    /* 0x376 */ s16 unk_376;
    /* 0x378 */ s16 unk_378;
    /* 0x37C */ MsgScriptCallback msgScriptCallback;
    /* 0x380 */ s32 unk_380;
    /* 0x384 */ s32 unk_384;
    /* 0x388 */ s32 prevTalkState;
    /* 0x38C */ s32 unk_38C;
    /* 0x390 */ UNK_TYPE1 unk_390[0x4];
    /* 0x394 */ s32 unk_394;
    /* 0x398 */ s32 unk_398;
    /* 0x39C */ UNK_TYPE1 unk_39C[0x4];
} EnPm; // size = 0x3A0

static MsgScript D_80AFB5A0[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(PACK_WEEKEVENTREG_FLAG(86, 0x08),
                                                       0x004A - 0x0005),
    /* 0x0005 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0008 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0009 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x28BA),
    /* 0x000C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000D 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0010 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0011 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x28BB),
    /* 0x0014 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0015 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0018 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0019 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x28BC),
    /* 0x001C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001D 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0020 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0021 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x28BD),
    /* 0x0024 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0025 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0028 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0029 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x28BE),
    /* 0x002C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002D 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0030 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0031 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x28BF),
    /* 0x0034 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0035 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0038 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0039 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x28C0),
    /* 0x003C 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_ANJU),
    /* 0x003F 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0042 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0043 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0046 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(PACK_WEEKEVENTREG_FLAG(86, 0x08)),
    /* 0x0049 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x004A 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x004D 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x004E 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x295C),
    /* 0x0051 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0052 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0055 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0056 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x295D),
    /* 0x0059 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x005A 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x005D 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x005E 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x295E),
    /* 0x0061 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_ANJU),
    /* 0x0064 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0067 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0068 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x006B 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB60C[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2774),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB614[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2775),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB61C[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2777),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB624[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2778),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB62C[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2779),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB634[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2781),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB63C[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x277A),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB644[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2782),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_88_02),
    /* 0x000A 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB650[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2783),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB658[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_ITEM(ITEM_LETTER_MAMA, 0x001F - 0x0005),
    /* 0x0005 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_82_80, 0x0016 - 0x000A),
    /* 0x000A 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x277C),
    /* 0x000D 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0010 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0011 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_82_80),
    /* 0x0014 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0015 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0016 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x277D),
    /* 0x0019 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x001C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001D 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x001E 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x001F 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x277C),
    /* 0x0022 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0023 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x00FF),
    /* 0x0026 0x09 */ MSCRIPT_CMD_CHECK_ITEM_ACTION(PLAYER_IA_LETTER_MAMA, 0x0, 0x0047 - 0x002F, 0x0057 - 0x002F),
    /* 0x002F 0x03 */ MSCRIPT_CMD_PLAYER_TALK(0xFFFF),
    /* 0x0032 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0035 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x277E),
    /* 0x0038 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x003B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x003C 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_82_80),
    /* 0x003F 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_86_01),
    /* 0x0042 0x03 */ MSCRIPT_CMD_DELETE_ITEM(ITEM_LETTER_MAMA),
    /* 0x0045 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0046 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0047 0x03 */ MSCRIPT_CMD_PLAYER_TALK(0x277D),
    /* 0x004A 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x004B 0x03 */ MSCRIPT_CMD_JUMP_3(0x0),
    /* 0x004E 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0051 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0052 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_82_80),
    /* 0x0055 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0056 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0057 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x277D),
    /* 0x005A 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x005D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x005E 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_82_80),
    /* 0x0061 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0062 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB6BC[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0003 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27A5),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x000A 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x000B 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27A6),
    /* 0x000E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000F 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0012 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0013 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27A7),
    /* 0x0016 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0017 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x001A 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x001B 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27A8),
    /* 0x001E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001F 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0022 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0023 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27A9),
    /* 0x0026 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0027 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x002A 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x002B 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27AA),
    /* 0x002E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002F 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0032 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0033 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27AB),
    /* 0x0036 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0037 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x003A 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x003B 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27AC),
    /* 0x003E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x003F 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0042 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0043 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27AD),
    /* 0x0046 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0049 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x004C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x004D 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_89_08),
    /* 0x0050 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0053 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB710[] = {
    /* 0x0000 0x05 */ //MSCRIPT_CMD_CHECK_ITEM(ITEM_MASK_POSTMAN, 0x0028 - 0x0005),
    /* 0x0000 0x03 */ MSCRIPT_CMD_JUMP(0x2),
    /* 0x0003 0x01 */ 0x00,
    /* 0x0004 0x01 */ 0x00,
    /* 0x0005 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2780),
    /* 0x0008 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0009 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x27AE),
    /* 0x000C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000D 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x000E 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_MASK_POSTMAN, 0x0),
    /* 0x0013 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x0084),
    /* 0x0016 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_90_04),
    /* 0x0019 0x03 */ MSCRIPT_CMD_JUMP_3(0x0),
    /* 0x001C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT_DONE(),
    /* 0x001D 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0020 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_RECEIVED_POSTMANS_HAT),
    /* 0x0023 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0024 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_90_01),
    /* 0x0027 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0028 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2780),
    /* 0x002B 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x002E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002F 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_90_01),
    /* 0x0032 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0033 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB744[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2369),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB74C[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_PLAYER_TALK(0xFFFF),
    /* 0x0003 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0006 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x277E),
    /* 0x0009 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x000C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000D 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_82_80),
    /* 0x0010 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_86_01),
    /* 0x0013 0x03 */ MSCRIPT_CMD_DELETE_ITEM(ITEM_LETTER_MAMA),
    /* 0x0016 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0017 0x01 */ MSCRIPT_CMD_DONE(),
};

static MsgScript D_80AFB764[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_PLAYER_TALK(0x277D),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_82_80),
    /* 0x000A 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x000B 0x01 */ MSCRIPT_CMD_DONE(),
};

s32 func_80AF81E8(Actor* thisx, PlayState* play);
s32 func_80AF8348(Actor* thisx, PlayState* play);
s32 func_80AF8478(Actor* thisx, PlayState* play);

RECOMP_PATCH u8* func_80AF8540(EnPm* this, PlayState* play) {
    switch (this->scheduleResult) {
        case 28:
            this->msgScriptCallback = (MsgScriptCallback) func_80AF8348;
            return (u8*) D_80AFB6BC;

        case 29:
            return (u8*) D_80AFB710;

        case 16:
            this->msgScriptCallback = (MsgScriptCallback) func_80AF81E8;
            return (u8*) D_80AFB5A0;

        case 17:
            return (u8*) D_80AFB644;

        case 24:
            if (this->unk_356 & 0x2000) {
                this->msgScriptCallback = (MsgScriptCallback) func_80AF8478;
                return (u8*) D_80AFB74C;
            } else if (this->unk_356 & 0x4000) {
                return (u8*) D_80AFB764;
            } else {
                this->msgScriptCallback = (MsgScriptCallback) func_80AF8478;
                return (u8*) D_80AFB658;
            }
            break;

        case 25:
            return (u8*) D_80AFB650;

        default:
            if (Player_GetMask(play) == PLAYER_MASK_KAFEIS_MASK) {
                return (u8*) D_80AFB744;
            }

            switch (this->scheduleResult) {
                case 9:
                    return (u8*) D_80AFB614;

                case 14:
                    return (u8*) D_80AFB624;

                case 20:
                    return (u8*) D_80AFB61C;

                case 21:
                    return (u8*) D_80AFB634;

                case 22:
                    return (u8*) D_80AFB63C;

                case 18:
                case 74:
                case 75:
                case 76:
                case 77:
                case 78:
                case 79:
                    return (u8*) D_80AFB62C;

                default:
                    return (u8*) D_80AFB60C;
            }
            break;
    }
}