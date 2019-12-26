#include <ESProtocol.hpp>

#define RELAY_PIN 0

// Init ESProtocol
ESProtocol protocol;

// Define relay states
enum RelayState { RELAY_OPEN = HIGH, RELAY_CLOSE = LOW };

// To keep track of the relay's state
// we will store here globally.
RelayState currentRelayState;

/**
 * Set the state of the relay.
 * 
 * @param state the new state to be set.
 */
void setRelayState(RelayState state) {
  digitalWrite(RELAY_PIN, state);
  currentRelayState = state;
}

/**
 * Convert a state to a readable name.
 * 
 * @param state the state to convert.
 * @param name the destination buffer.
 */
void getStateName(RelayState state, char* name) {
  switch (state) {
  case RELAY_OPEN:
    strcpy(name, "open");
    break;
  case RELAY_CLOSE:
    strcpy(name, "close");
    break;
  default:
    strcpy(name, "unknown");
    break;
  }
}

void setup() {
  // Initialize the relay pin and set to opened by default
  pinMode(RELAY_PIN, OUTPUT);
  setRelayState(RELAY_OPEN);

  // Set up the ESProtocol
  protocol.setup();

  // Register RPC handler for "open" command
  protocol.addRPCHandler("open", [](JsonObject& params, JsonObject& result) {
    setRelayState(RELAY_OPEN);
  });

  // Register RPC handler for "close" command
  protocol.addRPCHandler("close", [](JsonObject& params, JsonObject& result) {
    setRelayState(RELAY_CLOSE);
  });

  // Register RPC handler for "getState" command
  protocol.addRPCHandler("getState", [](JsonObject& params, JsonObject& result) {
    char name[8];
    getStateName(currentRelayState, name);
    result["r"] = name;
  });
}

void loop() {
  protocol.loop();
}
