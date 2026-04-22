# JIRA Monitor — Automated Daily Pipeline

**Purpose:** Automated system that ingests JIRA issues from four engineering projects daily, processes them through a multi-model AI pipeline, and publishes structured summaries to the team dashboard.

**Trigger:** Scheduler fires once per day at 2 AM. If any step fails, the pipeline stops immediately and no partial results are published.

**Projects monitored:** CPC, SPAE, CPCB0, GZR

---

## Pipeline Overview

```
JIRA Server → Download → Process → Analyze → Digest → Upload → PAE Database → Summary Web
```

## Step 1 — Download

**What it does:** Pulls JIRA issues from the server and applies two layers of filtering before any AI processing begins.

#### Layer 1 — Project + Time Filter (server-side)

A JIRA Query Language query is sent to the JIRA server. Only issues updated within the last 7 days across the four monitored projects are returned. This filtering runs on the JIRA server — no unnecessary data is transferred locally.

#### Layer 2 — Team Whitelist Filter (local)

For each issue returned, the system collects all active participants: reporter, current assignee, and anyone who posted a comment within the lookback window. If none of them appear on the configured team whitelist, the issue is discarded and never downloaded.

This design keeps AI compute costs proportional to team activity, not raw JIRA volume.

**Output:** Raw issue data (title, description, type, status, priority, reporter, assignee, all comments) saved locally per issue.

---

## Step 2 — Process

**What it does:** Converts raw JIRA data into clean, AI-ready text. Three transformations are applied to every issue.

#### 2a. Format Conversion (no AI)

JIRA stores content in HTML. This step strips all HTML markup and converts it to plain structured text — preserving headings, lists, and code blocks. Deterministic, no model involved.

#### 2b. Image Captioning — Vision Model

Screenshots, diagrams, and error captures embedded in tickets are extracted and passed to the **Vision Model** along with surrounding text context. The model returns a plain-text description that replaces the image inline. This allows all downstream text-only steps to understand visual content such as PCIe link training diagrams, oscilloscope captures, and UI error screenshots.

#### 2c. Text Cleanup — Small LLM

If a description or comment body exceeds the input token threshold (typically large log dumps), the **Small LLM** is called to extract the key technical information and discard the rest. This step only fires for unusually large content — it does not run on normal-length issues.

**Output:** One normalized JSON file per issue containing cleaned description and all cleaned comments.

---

## Step 3 — Analyze

**What it does:** Reads each processed issue and produces a structured analysis report using the AI Engine.

#### Pre-filter (rule engine, no AI)

Before any LLM call, a rule engine scans each comment:

- **Noise removal** — Reactions (`+1`, `:thumbsup:`), automated bot messages, and `/lgtm` approvals are stripped. No LLM tokens spent on non-informative content.
- **Stage tagging** — Comments containing keywords like "root cause", "fixed in", "passed regression" are tagged with a workflow stage label (`RootCause`, `FixDeployment`, `Verification`) to help the LLM understand issue lifecycle context.

#### LLM Analysis — AI Engine (content / JSON)

The filtered and tagged content is sent to the **AI Engine** with a prompt tailored to the issue type:

| Issue Type | LLM Focus |
|---|---|
| Bug | Root cause, reproduction steps, fix status, regression risk |
| Story | Acceptance criteria, implementation progress, scope changes |
| Task | Deliverables, completion status, blocking issues |
| Epic | Strategic objectives, cross-team dependencies, milestone progress |

The AI Engine returns a structured JSON report:

| Field | Content |
|---|---|
| Background | What the issue is about and its context |
| Conclusion | Current state or resolution |
| Key Events | Significant developments |
| Timeline | Chronological events with timestamp and author |
| Unresolved | Outstanding items still requiring attention |
| Risks | Identified risks or concerns |
| Technical Context | Affected components, error patterns, performance impact |

For very large issues, content is processed in batches and results are merged iteratively.

**Output:** Structured JSON report + human-readable Markdown summary saved per issue.

---

## Step 4 — Digest

**What it does:** Compresses each full analysis report into a single identifying sentence for the dashboard listing view.

#### LLM Compression — AI Engine (1-sentence)

The complete Markdown analysis from Step 3 is sent to the **AI Engine** with a strict constraint: produce one sentence, maximum 50 tokens. The sentence must name concrete identifiers — component, drive model, customer, test platform — so engineers can recognize the issue at a glance without opening it.

Example output:
> PAE reported AtomosP drive failed with lane break test on Dell R7525 Gen4 U.2 server.

#### Transfer Page Assembly

A structured page object is assembled for each issue containing:
- Issue metadata: key, title, type, status, priority
- Participants: reporter and active comment authors
- `brief_summary` — the one-sentence digest from above
- `docs` — the full Markdown analysis from Step 3
- Timestamps: JIRA created, JIRA last updated, summary generated

**Output:** All assembled pages written to a local transfer file.

---

## Step 5 — Upload

**What it does:** Pushes all transfer pages to the PAE Database in a single atomic operation, then the Summary Web team dashboard reads from it.

#### Database Replacement (atomic)

Two operations run in sequence:

1. **Delete all** — every existing document in the `jiras` collection is removed
2. **Bulk insert** — all newly generated pages are inserted in one operation

This guarantees the dashboard reflects exactly the issues active on the day of the run. No stale data from resolved or de-prioritized tickets accumulates across runs.

**Output:** PAE Database refreshed → Summary Web team dashboard displays today's digests.

---

## Key Design Decisions

| Decision | Rationale |
|---|---|
| Full workspace reset on every run | Guarantees a clean daily snapshot; prevents stale data accumulating |
| Two-layer filter before any AI call | Server-side filter reduces API traffic; whitelist focuses LLM spend on team-relevant issues only |
| Rule engine before LLM analysis | Eliminates noise cheaply with pattern matching before spending tokens on inference |
| Analyze and Digest as separate steps | Analyze produces a rich structured report for deep reading; Digest produces a short identifier optimized for the listing view |
| Atomic database replacement | Simplest and most reliable way to ensure the dashboard always shows exactly today's results |
