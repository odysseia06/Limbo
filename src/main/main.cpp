#include <iostream>
#include "core/logger.h"

int main() {
	LIMBO_TRACE("Hello from Log");
    int playerScore = 500;
    LIMBO_INFO("Engine has started! Player score: {}", playerScore);
    float enemyHP = 77.5f;
    LIMBO_WARN("Enemy HP is down to {}", enemyHP);

	return 0;
}