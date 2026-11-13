//////////////////////////////////////////////////
              //  LICENSE  //
//////////////////////////////////////////////////
/*
  ************************************************************************************
  * MIT License
  *
  * Copyright (c) 2023 Crunchlabs LLC (IRTurret Control Code)
  * Copyright (c) 2020-2022 Armin Joachimsmeyer (IRremote Library)
  * Copyright (c) 2025 Jorge Matesanz Hernando (Modified for Serial Control)
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is furnished
  * to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
  * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
  * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
  * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  *
  ************************************************************************************
*/

#include <Arduino.h>
#include <Servo.h>

// Definición de Servos
Servo yawServo;   // Base (Izquierda/Derecha)
Servo pitchServo; // Altura (Arriba/Abajo)
Servo rollServo;  // Mecanismo de disparo

// Estado de los servos
int yawServoVal = 90;
int pitchServoVal = 90; // Posición Home
int rollServoVal = 90;

// Configuración de movimiento
const int PITCH_STEP = 2;       // 2 grados por comando
const int YAW_STEP_TIME = 10;   // 2 grados de movimieto aproximadamente
const int ROLL_PRECISION = 160;

// Velocidades de servos (90 es quieto para continua, o valor medio)
const int YAW_STOP_SPEED = 90;
const int YAW_MOVE_SPEED = 90;  // Diferencia desde 90 (90+90=180 max vel)
const int ROLL_STOP_SPEED = 90;
const int ROLL_MOVE_SPEED = 90;

// Límites físicos
const int PITCH_MAX = 150;
const int PITCH_MIN = 33;

void setup() {
    Serial.begin(9600);

    // Pines
    yawServo.attach(10);
    pitchServo.attach(11);
    rollServo.attach(12);

    Serial.println("--- TORRETA LISTA ---");
    Serial.println("Comandos Seriales:");
    Serial.println("  w: Arriba (+2 deg)");
    Serial.println("  s: Abajo (-2 deg)");
    Serial.println("  a: Izquierda");
    Serial.println("  d: Derecha");
    Serial.println("  f: Disparar (Fire)");
    Serial.println("  h: Home");

    homeServos();
}

void loop() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();

        switch(cmd) {
            case 'w':
            case 'W':
                moveUp();
                break;

            case 's':
            case 'S':
                moveDown();
                break;

            case 'a':
            case 'A':
                moveLeft();
                break;

            case 'd':
            case 'D':
                moveRight();
                break;

            case 'f':
            case 'F':
                fire();
                break;

            case 'h':
            case 'H':
                homeServos();
                break;
        }
        // Limpiamos buffer por si acaso envía \n
        delay(10);
    }
}

// --- FUNCIONES DE MOVIMIENTO ---

void moveUp() {
    if ((pitchServoVal + PITCH_STEP) <= PITCH_MAX) {
        pitchServoVal += PITCH_STEP;
        pitchServo.write(pitchServoVal);
        Serial.print("PITCH: "); Serial.println(pitchServoVal);
    } else {
        Serial.println("MAX PITCH ALCANZADO");
    }
}

void moveDown() {
    if ((pitchServoVal - PITCH_STEP) >= PITCH_MIN) {
        pitchServoVal -= PITCH_STEP;
        pitchServo.write(pitchServoVal);
        Serial.print("PITCH: "); Serial.println(pitchServoVal);
    } else {
        Serial.println("MIN PITCH ALCANZADO");
    }
}

void moveLeft() {
    // Para servos de rotación continua: Moverse un tiempo y parar
    yawServo.write(YAW_STOP_SPEED + YAW_MOVE_SPEED);
    delay(YAW_STEP_TIME);
    yawServo.write(YAW_STOP_SPEED);
    Serial.println("LEFT");
}

void moveRight() {
    yawServo.write(YAW_STOP_SPEED - YAW_MOVE_SPEED);
    // Compensar error servo (moverse 5ms más)
    delay(YAW_STEP_TIME + 5);
    yawServo.write(YAW_STOP_SPEED);
    Serial.println("RIGHT");
}

void fire() {
    Serial.println("FIRE!");

    for (int i = 0; i <= 10; i++){
      rollServo.write(ROLL_STOP_SPEED + ROLL_MOVE_SPEED);
      delay(ROLL_PRECISION);
      rollServo.write(ROLL_STOP_SPEED);
    }
}

void homeServos() {
    Serial.println("HOMING...");
    yawServo.write(YAW_STOP_SPEED);
    rollServo.write(ROLL_STOP_SPEED);

    pitchServoVal = 100; // Posición segura
    pitchServo.write(pitchServoVal);
    delay(500); // Dar tiempo a llegar
}
