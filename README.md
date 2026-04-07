# 📚 Programming Learning Analytics & Personalisation System (PLAPS)

A C++ prototype system developed for Asia Pacific University (APU) to simulate personalised programming learning, track student progress, manage learning sessions, log activities, and identify at-risk learners.

> No STL containers used (`<vector>`, `<list>`, `<queue>`, `<stack>`, `<priority_queue>`) — all data structures are custom-built.

---

## 📌 Project Overview

PLAPS is built around four core components, each handled by a team member using appropriate data structures:

| Task | Description | Data Structure |
|---|---|---|
| **Task 1** | Learner Registration & Session Management | Queue / Circular Queue |
| **Task 2** | Activity Navigation & Session Flow | Stack |
| **Task 3** | Recent Activity Logging & Performance History | Circular Queue |
| **Task 4** | At-Risk Learner Priority & Recommendation Engine | Priority Queue |

---

## ⚙️ Features

### Task 1 — Learner Registration & Session Management
- Register learners and manage entry into active sessions
- Handle full session capacity
- Allow learners to exit and free up session slots
- Display registration list and active session list

### Task 2 — Activity Navigation & Session Flow
- Structured sequence of learning activities
- Move forward to next activity
- "Go back" / undo to revisit previous activities (Stack)
- Record outcomes (score, topic, difficulty) and forward to logger

### Task 3 — Recent Activity Logging & Performance History
- Fixed-size circular log — old records auto-replaced when full
- View all activity logs or filter by specific learner
- Export activity records to CSV for analysis

### Task 4 — At-Risk Learner Priority & Recommendation Engine
- Calculate risk score based on scores, failed attempts, consistency, time vs progress
- Rank learners by risk level using Priority Queue
- Display highest-priority at-risk learners
- Attach recommended actions (repeat topic, easier activity)
- Export at-risk learner list to CSV

---

## 📁 File Structure
Assignment2 Final Version/
├── Activity.cpp / Activity.hpp       # Activity navigation logic
├── Analytic.cpp / Analytic.hpp       # Analytics & risk scoring
├── Array.cpp / Array.hpp             # Custom array data structure
├── Helper.cpp / Helper.hpp           # Utility/helper functions
├── LinkedList.cpp / LinkedList.hpp   # Custom linked list
├── Logger.cpp / Logger.hpp           # Circular log implementation
├── Question.cpp / Question.hpp       # Question/activity model
├── Session.cpp / Session.hpp         # Session queue management
├── Student.hpp                       # Learner data model
├── main.cpp                          # Driver program
├── activity_logs.csv                 # Activity log dataset
├── at_risk_learners.csv              # At-risk learner export
├── learner.csv                       # Learner records
├── logger.csv                        # Logger export
└── question.csv                      # Question dataset

---

## 🚀 How to Run
```bash
# Compile
g++ -o mySystem main.cpp Activity.cpp Analytic.cpp Array.cpp Helper.cpp LinkedList.cpp Logger.cpp Question.cpp Session.cpp

# Run
./mySystem
```

---

## 🛠️ Tech Stack

- Language: **C++**
- Data Structures: **Queue, Circular Queue, Stack, Priority Queue** (all custom-built)
- No STL containers used

---

## 👥 Team Members

| Member | Task |
|---|---|
| Chan Xin Quan | Task 1 — Learner Registration & Session Management |
| Sawraw Madub | Task 2 — Activity Navigation & Session Flow |
| Chen Zhen Hao | Task 3 — Recent Activity Logging & Performance History |
| Kaemon Ng | Task 4 — At-Risk Learner Priority & Recommendation Engine |
