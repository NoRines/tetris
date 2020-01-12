#ifndef INPUT_H
#define INPUT_H

#include <cstdint>

void openController();
void closeController();

void updateOldMaps();

void setKeyState(uint32_t key, bool state);

bool isKeyDown(uint32_t key);
bool isKeyPressed(uint32_t key);
bool isKeyReleased(uint32_t key);

void setControllerButtonState(uint8_t button, bool state);

bool isControllerButtonDown(uint8_t button);
bool isControllerButtonPressed(uint8_t button);
bool isControllerButtonReleased(uint8_t button);

#endif
