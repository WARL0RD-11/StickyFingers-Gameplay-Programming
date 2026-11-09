# Sticky Fingers — Gameplay Programming Showcase

**Unreal Engine 5 | C++ | Gameplay Systems | Nintendo Switch & PC**

A technical portfolio repository showcasing my individual gameplay programming contributions to **Sticky Fingers**, a couch co-op heist game developed by a multidisciplinary team of 15 developers at the University of Utah.

This repository focuses on the engineering behind the gameplay experience: modular interaction systems, cross-platform input handling, user interface functionality, and developer-facing tools.

Rather than presenting the entire game codebase, this showcase highlights selected systems I worked on, their architectural decisions, and the engineering challenges encountered during development.

**[View Sticky Fingers on Steam](https://store.steampowered.com/app/4577120/Sticky_Fingers/)**

---

## Project Overview

Sticky Fingers is a couch co-op heist game in which two players work together to control a cat burglar's hands, manipulate objects, and navigate increasingly chaotic heists.

The project was developed between **August 2025 and April 2026**, with Nintendo Switch as a primary target alongside PC.

### My Role

**Gameplay Programmer | Audio Composer**

My gameplay programming responsibilities included:

- Designing and implementing modular object interaction and grabbing systems.
- Developing cross-platform input handling for different controller configurations.
- Implementing context-sensitive UI prompts and shared menu navigation.
- Building and integrating gameplay functionality for a slot machine.
- Developing debugging and developer-facing tools.
- Supporting gameplay integration, troubleshooting, and platform-specific behavior.

My work focused on maintaining clear separation between gameplay responsibilities, reducing unnecessary coupling, and creating systems that could be reused or extended as development progressed.

---

## Technical Contributions

### 1. Modular Interaction System

**Focus:** Player interactions, object manipulation, and reusable gameplay behavior.

Developed gameplay functionality supporting grabbing, releasing, and interacting with objects within the cooperative gameplay environment.

Key engineering considerations:

- Separating interaction detection from the resulting gameplay action.
- Maintaining consistent interaction behavior across supported input devices.
- Supporting reusable interaction functionality across different gameplay objects.
- Communicating interaction availability through UI feedback.

**Engineering emphasis:** Modularity, responsibility separation, extensibility, and player responsiveness.

### 2. Cross-Platform Input Systems

**Focus:** Input-device handling and controller compatibility.

Implemented functionality to support input workflows across Nintendo Switch controllers, Xbox-style controllers, and keyboard/mouse configurations.

Key engineering considerations:

- Runtime identification of active input devices.
- Supporting platform-specific input behavior.
- Coordinating input changes with gameplay and UI systems.
- Maintaining consistent player-facing behavior across device configurations.

**Engineering emphasis:** Input abstraction, cross-platform compatibility, and maintainability.

### 3. User Interface & Player Feedback

**Focus:** Context-sensitive prompts, navigation, and player communication.

Worked on systems that connect gameplay interaction states with the player-facing interface.

Key engineering considerations:

- Displaying appropriate interaction prompts.
- Updating UI behavior based on the active input device.
- Supporting shared menu navigation.
- Keeping gameplay logic and presentation responsibilities appropriately separated.

**Engineering emphasis:** Event-driven communication, UI consistency, and usability.

### 4. Slot Machine Gameplay System

**Focus:** Self-contained gameplay mechanics and system integration.

Implemented gameplay functionality for an interactive slot machine featured in Sticky Fingers.

Areas of implementation included:

- Gameplay logic and feature behavior.
- Integration with the game's interaction workflow.
- Coordination between gameplay states and player-facing feedback.
- Debugging and gameplay iteration.

**Engineering emphasis:** Feature encapsulation, state management, and system integration.

### 5. Debugging & Developer Tools

**Focus:** Improving development efficiency and gameplay testing.

Developed and integrated internal debugging features to make gameplay behavior easier to inspect and test during development.

Key objectives:

- Simplifying repeated gameplay testing.
- Providing developer-oriented controls and diagnostics.
- Supporting faster issue reproduction.
- Reducing iteration time during feature integration.

**Engineering emphasis:** Debuggability, developer experience, and production workflows.

---

## Engineering Philosophy

Throughout development, I focused on three guiding principles:

**1. Maintainable Gameplay Architecture**

Build systems with well-defined responsibilities so that individual features can be modified without introducing unnecessary dependencies across unrelated systems.

**2. Designer-Friendly Implementation**

Ensure gameplay functionality remains practical to configure, test, and iterate on within Unreal Engine's development environment.

**3. Player-Focused Engineering**

Prioritize responsive interactions, reliable input handling, and clear gameplay feedback, particularly in a cooperative experience where multiple players interact with shared systems.

These principles continue to guide my approach to gameplay architecture and production engineering.

---

## Technology

**Engine:** Unreal Engine 5

**Primary Language:** C++

**Additional Systems:** Unreal Engine Blueprints, Enhanced Input, UMG

**Platforms:** Nintendo Switch, Windows PC

**Version Control:** Perforce

**Development Environment:** Visual Studio

---

## Source Code Scope

This repository is intended as a curated technical portfolio rather than a complete Unreal Engine project.

It may contain selected C++ implementations, technical documentation, and original diagrams demonstrating my individual contributions.

The complete Sticky Fingers source repository remains private.

As a result:

- The included code may not compile independently without its original project dependencies.
- Some implementation details may be omitted or adapted for presentation.
- Third-party assets, proprietary project content, and other developers' implementations are excluded.
- Only source material authorized for public distribution will be published.

The objective is to demonstrate gameplay engineering experience while respecting the original project's ownership and collaborative nature.

---

## Credits & Attribution

**Sticky Fingers** was developed by a multidisciplinary team of 15 developers at the University of Utah.

This repository documents my individual gameplay programming work and does not claim ownership of the complete game or the contributions of other team members.

My contributions were developed within a collaborative production environment involving gameplay programmers, designers, artists, and other disciplines.

All rights to the original game, its assets, and its complete source code remain with their respective rights holders.

---

## About Me

I'm **Nishant Verma**, a Gameplay Programmer specializing in Unreal Engine and C++.

My interests include gameplay architecture, reusable gameplay systems, artificial intelligence, multiplayer programming, and development tools.

I enjoy building systems that are maintainable for engineers, configurable for designers, and responsive for players.

**[GitHub](https://github.com/WARL0RD-11)**

---

*This repository is a technical portfolio showcase of selected individual contributions. It is not an official distribution of Sticky Fingers or its complete source code.*