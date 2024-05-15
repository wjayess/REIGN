#pragma once

enum bones : int
{
    head = 6,
    neck = 5,
    spine = 4,
    spine_1 = 2,
    leftShoulder = 8,
    leftArm = 9,
    leftHand = 11,
    pelvis = 0,
    rightShoulder = 13,
    rightArm = 14,
    rightHand = 16,
    leftHip = 22,
    leftKnee = 23,
    leftFeet = 24,
    rightHip = 25,
    rightKnee = 26,
    rightFeet = 27
};

struct BoneConnection
{
    int bone1;
    int bone2;

    BoneConnection(int b1, int b2) : bone1(b1), bone2(b2) {}
};

BoneConnection bConnectionArray[] = {
    BoneConnection(6, 5),  // head to neck
    BoneConnection(5, 4),  // neck to spine
    BoneConnection(4, 0),  // spine to pelvis
    BoneConnection(4, 8), // spine to leftShoulder
    BoneConnection(8, 9), // leftShoulder to leftArm
    BoneConnection(9, 11), // arm to hand
    BoneConnection(4, 13),
    BoneConnection(13, 14),
    BoneConnection(14, 16),
    BoneConnection(4, 2),  // spine to spine_1
    BoneConnection(0, 22), // pelvis to leftHip
    BoneConnection(0, 25), // pelvis to rightHip
    BoneConnection(22, 23), // leftHip to leftKnee
    BoneConnection(23, 24), // leftKnee to leftFoot
    BoneConnection(25, 26), // rightHip to rightKnee
    BoneConnection(26, 27) // rightKnee to rightFoot
};