L3 (KR̥ṢṬ) — FOUNDATION DOCUMENT
===============================

This document is the final, non-negotiable foundation of L3.
It defines roles, laws, flow, and future guarantees.
Implementation may evolve. Behavior must not.

----------------------------------------------------------------
1. PURPOSE
----------------------------------------------------------------
L3 is the intelligence orchestration layer of ShrijiLang.

It separates:
- Thinking (logic, calculation)
- Speaking (human-facing output)
- Teaching (explicit learning mode)

L3 is designed to work today with lightweight logic
and tomorrow with machine-backed, high-compute intelligence
without breaking language behavior.

----------------------------------------------------------------
2. CORE ROLES (FROZEN)
----------------------------------------------------------------

Router
------
- Central authority of L3
- Decides intent
- Controls full request lifecycle
- Chooses which module participates

Router never:
- Thinks deeply
- Formats language
- Teaches directly

Niyu (THINK)
------------
- Silent thinking engine
- Performs logic, reasoning, calculation
- Produces internal result only

Niyu never:
- Speaks to user
- Decides intent
- Formats language

Sakhi (SPEAK)
-------------
- Final voice of the system
- Produces human-readable output
- Applies tone and clarity

Sakhi never:
- Thinks or calculates
- Decides intent
- Teaches

Mira (TEACH)
------------
- Teaching and explanation layer
- Activated only when explicitly allowed
- Provides structured learning responses

Mira never:
- Handles normal conversation
- Decides intent
- Performs deep logic

----------------------------------------------------------------
3. FLOW (CANONICAL)
----------------------------------------------------------------

User Input
   |
   v
Router
   |
   |-- decide intent
   |-- check teaching_mode
   |
   |--> Niyu (if logic / calculation needed)
   |        |
   |        v
   |   NiyuResult (internal)
   |
   |--> Sakhi (normal response)
   |        OR
   |--> Mira  (teaching response)
   |
   v
Final L3Response to User

No module may bypass the Router.

----------------------------------------------------------------
4. CONTRACTS (IMMUTABLE)
----------------------------------------------------------------
- l3_request.h  : defines input shape
- l3_intent.h   : defines intent space
- l3_response.h : defines output shape
- l3_rules.h    : defines laws

Contracts are stable forever.
Engines may change. Contracts must not.

----------------------------------------------------------------
5. NON-NEGOTIABLE LAWS
----------------------------------------------------------------
1. Router is the sole authority.
2. Niyu thinks, never speaks.
3. Sakhi speaks, never thinks.
4. Mira teaches only when explicitly enabled.
5. No direct Niyu <-> Sakhi communication.
6. All flows pass through Router.
7. Behavior consistency is mandatory.
8. Future machine intelligence must fit this structure.

----------------------------------------------------------------
6. FUTURE GUARANTEE
----------------------------------------------------------------
L3 is intentionally designed as a replaceable intelligence layer.

Today:
- Local logic
- Lightweight reasoning

Tomorrow:
- Dedicated machines
- Heavy computation
- Real-world modeling

The language behavior, contracts, and flow
must remain unchanged across this evolution.

----------------------------------------------------------------
7. FINAL NOTE
----------------------------------------------------------------
This document is the NĒEV (foundation) of L3.

Any change that violates this document
is considered a breaking change and is not allowed.

Implementation follows this document.
This document never follows implementation.
