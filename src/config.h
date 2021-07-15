#ifndef WP_CONFIG
#define WP_CONFIG

typedef struct Panel Pad;
struct Panel {
    uint32_t threshold;
    uint8_t stepOrder;
    uint8_t gamepadButton;
    uint8_t teensyPin;
    uint8_t ledOrder;
    uint32_t value;
    bool pressed;
};

// Panel configuration

Panel PANELS[] = {
    { // Left arrow
        230, // threshold
        3,   // stepOrder
        1,   // gamepadButton
        A3,  // teensyPin
        3    // ledOrder
    }, { // Up arrow
        285, // threshold
        0,   // stepOrder
        2,   // gamepadButton
        A2,  // teensyPin
        0    // ledOrder
    }, { // Down arrow
        230, // threshold
        2,   // stepOrder
        3,   // gamepadButton
        A1,  // teensyPin
        2    // ledOrder
    }, { // Right arrow
        90, // threshold
        1,   // stepOrder
        4,   // gamepadButton
        A0,  // teensyPin
        1    // ledOrder
    }
};

#define LED_PIN 10
#define LEDS_PER_PAD 32
#define SERIAL_BAUD_RATE 2000000


#define LED_COUNT LEDS_PER_PAD*len(PANELS)
#define NUM_STEPS (sizeof PANELS / sizeof PANELS[0])

#define ANALOGREAD_SMOOTHING_SAMPLES 16 // approx 0.5ms latency

#endif
