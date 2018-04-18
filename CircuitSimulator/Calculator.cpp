#include "Calculator.h"

bool Calculator::isCompleteCircuit(spot_vec& vec, int numComponents) {
    bool isComplete = numComponents > 0;

    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec[0].size(); j++) {
            if (vec[i][j]->getComponents().size() == 1)
                isComplete = false;
        }
    }

    return isComplete;
}