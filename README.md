# SaguniStudio_AI
Modular Combat AI System in Unreal Engine 5.4

## 🚀 Overview
This project is a prototype AI system designed in Unreal Engine 5.4 that demonstrates intelligent enemy behavior, group coordination, and adaptive combat logic. The system simulates a group of five AI enemies engaging in strategic combat against the player using modular AI architecture, combining the power of C++ and Blueprints.

## 🎯 Assessment Task Objectives
Develop a modular AI system with intelligent and cooperative enemy behaviors.

Implement at least two distinct attacks per enemy with telegraphed cues.

Use UE5’s AI Perception (Sight + Hearing) to respond to player actions.

Coordinate group tactics, attack rotations, and strategic movement.

Handle cooldown management, retreating, and adaptive difficulty.

Prioritize C++ for core systems with Blueprint integration for flexibility.

## 🛠️ Features


### ⚔️ Enemy Class & Attack System
Each AI enemy has:

Two unique attacks (light + heavy) with variable telegraphs.

Attack selection based on real-time conditions (distance, player state, cooldowns).

---

### 👁️ Perception & Pathfinding
Integrated AI Perception System:

Sight & hearing responses for detecting and reacting to the player.

Intelligent movement via NavMesh pathfinding.

---

### 🧠 Group Coordination
Group of 5 AI enemies:

Coordinate to take turns attacking.

Others reposition, flank, or support during cooldowns.

Optional extensions:

Flanking maneuvers.

Support-based positioning.

Combo attacks with other AI units.

---

### 🎮 Combat Behavior
Dynamic state machine & behavior tree implementation.

AI retreat and re-engagement based on:

Health level

Cooldown status

Player distance

Rotating attack priority to avoid spamming single actions.

