#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnGeg;

typedef void (*EnGegActionFunc)(struct EnGeg*, PlayState*);

#define ENGEG_FIDGET_TABLE_LEN 3

#define GORON_LIMB_MAX 0x12

typedef struct EnGeg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnGegActionFunc actionFunc;
    /* 0x18C */ ColliderCylinder colliderCylinder;
    /* 0x1D8 */ ColliderSphere colliderSphere;
    /* 0x230 */ u16 unk_230;
    /* 0x232 */ s16 fidgetTableZ[ENGEG_FIDGET_TABLE_LEN];
    /* 0x238 */ s16 fidgetTableY[ENGEG_FIDGET_TABLE_LEN];
    /* 0x23E */ s16 unk_23E;
    /* 0x240 */ s16 unk_240;
    /* 0x242 */ s16 unk_242;
    /* 0x244 */ s16 unk_244;
    /* 0x248 */ s32 objectSlot;
    /* 0x24C */ Vec3s jointTable[GORON_LIMB_MAX];
    /* 0x2B8 */ Vec3s morphTable[GORON_LIMB_MAX];
    /* 0x324 */ UNK_TYPE1 unk324[0x144];
    /* 0x468 */ s16 unk_468;
    /* 0x46A */ s16 unk_46A;
    /* 0x46C */ s16 unk_46C;
    /* 0x46E */ s16 unk_46E;
    /* 0x470 */ Vec3f unk_470;
    /* 0x47C */ Vec3s unk_47C;
    /* 0x484 */ Vec3f unk_484;
    /* 0x490 */ Vec3s unk_490;
    /* 0x496 */ u16 unk_496;
    /* 0x498 */ s16 csId;
    /* 0x49A */ s16 nextCsId;
    /* 0x49C */ s16 csIdList[8];
    /* 0x4AC */ s32 animIndex;
    /* 0x4B0 */ s16 unk_4B0;
    /* 0x4B4 */ Vec3f unk_4B4;
    /* 0x4C0 */ Vec3f unk_4C0[2];
    /* 0x4D8 */ s16 unk_4D8;
    /* 0x4DC */ s32 unk_4DC;
    /* 0x4E0 */ s16 unk_4E0;
    /* 0x4E4 */ Vec3f unk_4E4;
} EnGeg; // size = 0x4F0

void func_80BB32AC(EnGeg* this, PlayState* play);

RECOMP_PATCH void func_80BB31B8(EnGeg* this, PlayState* play) {
    s32 getItemId = GI_MASK_DON_GERO;

    // if (INV_CONTENT(ITEM_MASK_DON_GERO) == ITEM_MASK_DON_GERO) {
    //     if (Player_GetMask(play) == PLAYER_MASK_DON_GERO) {
    //         this->unk_496 = 0xD8B;
    //         getItemId = GI_RUPEE_PURPLE;
    //     } else {
    //         this->unk_496 = 0xD73;
    //         getItemId = GI_RUPEE_PURPLE;
    //     }
    // } else {
    //     this->unk_496 = 0xD70;
    // }
    this->unk_496 = 0xD70;

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        SET_WEEKEVENTREG(WEEKEVENTREG_61_01);
        if (getItemId == GI_MASK_DON_GERO) {
            this->unk_230 |= 0x40;
        }
        this->actionFunc = func_80BB32AC;
    } else {
        Actor_OfferGetItem(&this->actor, play, getItemId, 300.0f, 300.0f);
    }
}