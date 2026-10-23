# DeepBugger Extension — Agent Operating Rules (`AGENTS.md`)

This document establishes the permanent, mandatory operating rules for all Antigravity work on the DeepBugger Extension research repository. Every future interaction must strictly comply with these rules.

**Research Source of Truth:** `DeepBugger_Extension_Final_Master_Plan_v4.md`

Do not silently invent research specifications that are not defined in the Master Plan. If the Master Plan explicitly says that something is unresolved, keep it unresolved until confirmed by the user.

---

## 1. One micro-project at a time

Work on exactly ONE clearly defined task per interaction.

Never combine multiple implementation objectives into one task.

Never automatically continue to the next research step after completing the current one.

At the end of every task, STOP and wait for user approval.

Do not use parallel agents or subagents unless the user explicitly requests them.

---

## 2. Read before editing

Before modifying code:

1. Read this `AGENTS.md`.
2. Read the relevant section of:
   `DeepBugger_Extension_Final_Master_Plan_v4.md`
3. Inspect only the files necessary for the current task.
4. Understand the existing implementation before changing it.

Do not perform broad refactoring simply because you think the code could be improved.

---

## 3. Never guess research specifications

If a research-relevant detail is missing, ambiguous, inconsistent, or unsupported by the Master Plan or existing verified code:

STOP.

Ask the user a focused question before implementing it.

Examples include:

- exact feature definitions (e.g., exact sub-dimensions of the +7-D node feature extension);
- unresolved D3 “5 types” (`5 types × 600`);
- lineage definitions and canonical source grouping rules;
- dataset labels, common-library policy, and annotation rules;
- thresholds;
- model architecture details not specified in the plan;
- loss settings (e.g., exact $\lambda_{\text{lin}}$ scheduling/values if unspecified);
- data splits;
- evaluation rules (including keeping extraction failures in end-to-end denominators);
- changes that could alter historical Mini-DeepBugger results.

Never invent a value just to make the code run.

---

## 4. Preserve the Mini-DeepBugger baseline

The existing Mini-DeepBugger/APSEC implementation is the historical baseline.

Do not silently change:

- the 29-D baseline representation;
- baseline datasets;
- historical experiment manifests;
- baseline thresholds;
- baseline evaluation logic;
- historical result files.

Extension work must be isolated from the frozen baseline whenever possible.

If a requested change could modify baseline behavior, STOP and ask first.

---

## 5. TARGET values are not RESULTS

Values in the professor's proposal marked or understood as targets (`TARGET ≠ RESULT`) must never be reported as measured experimental results.

Use measured results only after an experiment has actually been executed and verified.

If measured evidence contradicts the planned hypothesis, report the measured evidence.

Never modify evaluation data or logic to force the expected result.

---

## 6. Minimal changes only

For every task:

- change the smallest possible number of files;
- avoid unrelated cleanup;
- avoid unrelated formatting;
- avoid dependency upgrades unless explicitly required;
- avoid deleting existing files;
- avoid renaming public interfaces unless explicitly approved.

Do not modify anything outside the scope of the current micro-project.

---

## 7. Ask before risky or ambiguous actions

STOP and ask before:

- deleting files;
- overwriting datasets;
- changing train/dev/test splits;
- changing research labels;
- changing baseline behavior;
- changing dependency versions;
- performing large refactors;
- downloading large datasets;
- running expensive model training;
- modifying Git history;
- force pushing;
- pushing to GitHub;
- changing Git remotes;
- using credentials;
- making an architectural decision not defined by the Master Plan.

---

## 8. Verification is mandatory

A task is not complete simply because code was written.

Before declaring success:

1. inspect the diff;
2. run the smallest relevant verification;
3. run tests if relevant tests already exist;
4. confirm no unrelated files changed;
5. confirm the current task's acceptance criteria are satisfied.

If verification fails:

- fix only problems inside the current scope;
- if the failure requires a broader change, STOP and ask the user.

Never hide a failing check.

---

## 9. Project progress tracking

Maintain:

`PROJECT_PROGRESS.md`

After every completed micro-project, append a new entry using this structure:

### STEP-XXX — <short task name>

- Date:
- Status: DONE
- Goal:
- Master Plan section:
- Files inspected:
- Files changed:
- Commands executed:
- Verification performed:
- Result:
- Decisions made:
- Open questions:
- Git commit:
- Next proposed micro-project:

If a task becomes blocked, add:

- Status: BLOCKED
- Blocking reason:
- Question requiring user decision:

Never mark a task DONE unless verification succeeded.

---

## 10. Git discipline

Before starting a task:

Run:

`git status`

If there are unexpected uncommitted changes that could conflict with the task, STOP and ask the user what to do.

After successfully completing and verifying a micro-project:

1. show `git status`;
2. inspect `git diff`;
3. update `PROJECT_PROGRESS.md`;
4. stage ONLY files belonging to the current task;
5. create ONE focused local commit.

Commit message format:

`step-XXX: <short description>`

Example:

`step-001: audit Mini-DeepBugger repository`

Do not combine unrelated changes in one commit.

Do NOT push automatically.

Prepare the repository so the commit can be pushed to GitHub, but wait for explicit user instruction before running `git push`.

If Git is not initialized, no remote exists, or Git identity is missing, STOP and ask the user before changing Git configuration.

---

## 11. Completion report

At the end of every micro-project, respond with exactly these sections:

### Completed
Briefly state what was completed.

### Files Changed
List changed files.

### Verification
State exactly what was checked and whether it passed.

### Git
Provide branch name and commit hash if committed.

### Questions
List unresolved questions, or `None`.

### Next Proposed Micro-Project
Propose ONE small next task only.

Then STOP.

Do not execute the proposed next task until the user approves it.
