/*
 * Project 3: Callback-Based Device Monitoring Simulator
 * Student Email: d.akol@alustudent.com
 * Custom Callback: fault_detector()
 * Device types: Temperature Sensor, Pressure Sensor, Voltage Monitor
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Device type constants */
#define TYPE_TEMPERATURE  0
#define TYPE_PRESSURE     1
#define TYPE_VOLTAGE      2

/* Device structure with union for different reading types */
struct Device {
    char name[30];
    int  type;
    union {
        float temperature;  /* degrees Celsius */
        int   pressure;     /* kilopascals     */
        float voltage;      /* volts           */
    } reading;
};

/* Callback 1: temperature monitor */
void temperature_monitor(struct Device *d) {
    printf("  [TEMP]      %-20s  reading = %.1f C", d->name, d->reading.temperature);
    if      (d->reading.temperature > 80.0f)  printf("  *** OVERHEAT ALERT ***");
    else if (d->reading.temperature < -10.0f) printf("  ** FREEZE WARNING **");
    else                                        printf("  [Normal]");
    printf("\n");
}

/* Callback 2: pressure monitor */
void pressure_monitor(struct Device *d) {
    printf("  [PRESSURE]  %-20s  reading = %d kPa", d->name, d->reading.pressure);
    if      (d->reading.pressure > 200) printf("  *** HIGH PRESSURE ***");
    else if (d->reading.pressure < 50)  printf("  ** LOW PRESSURE **");
    else                                 printf("  [Normal]");
    printf("\n");
}

/* Callback 3: battery/voltage monitor */
void battery_monitor(struct Device *d) {
    printf("  [VOLTAGE]   %-20s  reading = %.2f V", d->name, d->reading.voltage);
    if      (d->reading.voltage > 13.5f) printf("  *** OVERVOLTAGE ***");
    else if (d->reading.voltage < 10.5f) printf("  ** LOW BATTERY **");
    else                                  printf("  [Normal]");
    printf("\n");
}

/*
 * Custom callback: fault_detector
 * Assigns a severity level (OK, WARNING, CRITICAL) based on the
 * device type and its current reading, then prints a unified report.
 */
void fault_detector(struct Device *d) {
    int fault = 0;
    char msg[80] = "OK";

    if (d->type == TYPE_TEMPERATURE) {
        float t = d->reading.temperature;
        if      (t > 90.0f  || t < -20.0f) { fault = 2; strcpy(msg, "CRITICAL TEMP FAULT"); }
        else if (t > 80.0f  || t < -10.0f) { fault = 1; strcpy(msg, "TEMP WARNING"); }
    } else if (d->type == TYPE_PRESSURE) {
        int p = d->reading.pressure;
        if      (p > 250 || p < 30) { fault = 2; strcpy(msg, "CRITICAL PRESSURE FAULT"); }
        else if (p > 200 || p < 50) { fault = 1; strcpy(msg, "PRESSURE WARNING"); }
    } else {
        float v = d->reading.voltage;
        if      (v > 14.5f || v < 9.0f)  { fault = 2; strcpy(msg, "CRITICAL VOLTAGE FAULT"); }
        else if (v > 13.5f || v < 10.5f) { fault = 1; strcpy(msg, "VOLTAGE WARNING"); }
    }

    const char *sev = (fault == 2) ? "CRITICAL" : (fault == 1) ? "WARNING " : "OK      ";
    printf("  [FAULT-DET] %-20s  Severity: %s  |  %s\n", d->name, sev, msg);
}

/* Dispatcher: calls the given callback on the device */
void process_device(struct Device *d, void (*callback)(struct Device *)) {
    callback(d);   /* invoke the callback */
}

/* Generates a random reading appropriate to the device type */
void simulate_reading(struct Device *d) {
    if      (d->type == TYPE_TEMPERATURE)
        d->reading.temperature = -20.0f + (float)(rand() % 1200) / 10.0f;
    else if (d->type == TYPE_PRESSURE)
        d->reading.pressure    = 20 + rand() % 260;
    else
        d->reading.voltage     = 8.0f + (float)(rand() % 70) / 10.0f;
}

int main(void) {
    srand((unsigned)time(NULL));
    printf("Callback-Based Device Monitor\n\n");

    /* dynamically allocate array of devices */
    int num_devices = 6;
    struct Device *devices = malloc(num_devices * sizeof(struct Device));
    if (!devices) { fprintf(stderr, "malloc failed\n"); return 1; }

    /* initialise devices using pointer traversal */
    struct Device *p = devices;
    strcpy(p->name, "TempSensor-A");    p->type = TYPE_TEMPERATURE; p++;
    strcpy(p->name, "TempSensor-B");    p->type = TYPE_TEMPERATURE; p++;
    strcpy(p->name, "PressureSensor-1");p->type = TYPE_PRESSURE;    p++;
    strcpy(p->name, "PressureSensor-2");p->type = TYPE_PRESSURE;    p++;
    strcpy(p->name, "BatteryUnit-X");   p->type = TYPE_VOLTAGE;     p++;
    strcpy(p->name, "BatteryUnit-Y");   p->type = TYPE_VOLTAGE;

    /* function pointer array indexed by device type */
    void (*monitors[3])(struct Device *) = {
        temperature_monitor,
        pressure_monitor,
        battery_monitor
    };

    /* simulate 10 random readings */
    printf("=== Simulating 10 Random Readings ===\n\n");
    for (int round = 1; round <= 10; round++) {
        int idx = rand() % num_devices;
        struct Device *dev = &devices[idx];   /* pointer access */
        simulate_reading(dev);

        printf("  Round %2d: ", round);
        process_device(dev, monitors[dev->type]);   /* callback dispatch */
    }

    /* run fault detector across all devices */
    printf("\n=== Fault Detector Sweep ===\n");
    p = devices;
    for (int i = 0; i < num_devices; i++, p++) simulate_reading(p);

    p = devices;
    for (int i = 0; i < num_devices; i++, p++)
        process_device(p, fault_detector);   /* fault_detector callback */

    /* interactive menu */
    int choice;
    do {
        printf("\n--- MENU ---\n");
        printf("  1. Run another 10 random readings\n");
        printf("  2. Fault Detector sweep\n");
        printf("  3. Show all device names\n");
        printf("  0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("\n");
            for (int round = 1; round <= 10; round++) {
                int idx = rand() % num_devices;
                struct Device *dev = &devices[idx];
                simulate_reading(dev);
                printf("  Round %2d: ", round);
                process_device(dev, monitors[dev->type]);
            }
        } else if (choice == 2) {
            printf("\n");
            p = devices;
            for (int i = 0; i < num_devices; i++, p++) {
                simulate_reading(p);
                process_device(p, fault_detector);
            }
        } else if (choice == 3) {
            printf("\n  Registered devices:\n");
            p = devices;
            for (int i = 0; i < num_devices; i++, p++)
                printf("    %d. %s\n", i + 1, p->name);
        }
    } while (choice != 0);

    free(devices);   /* release dynamic memory */
    printf("Goodbye.\n");
    return 0;
}

