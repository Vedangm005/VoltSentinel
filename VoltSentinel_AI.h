#ifndef VOLTSENTINEL_AI_H
#define VOLTSENTINEL_AI_H

#include "voltsentinel_model.h"


// ============================================================
// Run one Extra Trees decision tree
// ============================================================

float runTree(const float *features, int treeId) {

    int node = 0;

    while (true) {

        int feature = VS_TREE_FEATURE[treeId][node];

        // Leaf node
        if (feature == -2) {
            return VS_TREE_VALUE[treeId][node];
        }

        float threshold = VS_TREE_THRESHOLD[treeId][node];

        if (features[feature] <= threshold) {
            node = VS_TREE_LEFT[treeId][node];
        }
        else {
            node = VS_TREE_RIGHT[treeId][node];
        }
    }
}


// ============================================================
// Run complete 50-tree Extra Trees model
// ============================================================

float predictSOH(const float *features) {

    float total = 0.0f;

    for (int tree = 0; tree < VS_NUM_TREES; tree++) {

        total += runTree(features, tree);
    }

    float soh = total / VS_NUM_TREES;

    // Safety clamp
    if (soh < 0.0f) {
        soh = 0.0f;
    }

    if (soh > 100.0f) {
        soh = 100.0f;
    }

    return soh;
}

#endif