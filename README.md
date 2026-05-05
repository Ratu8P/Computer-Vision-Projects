Description: An automated daily pipeline that pulls JIRA issues from four engineering projects (CPC, SPAE, CPCB0, GZR), processes them through a multi-modal LLM pipeline, and publishes structured summaries to the team
  dashboard. Currently the output format is flat and undifferentiated, requiring manual effort to categorize and extract key information.

  Formula: Bug-type issues achieve ≥ 80% root_cause field population rate, summaries are generated across three distinct templates (Bug / Feature / Task), and one weekly Executive Summary is produced aggregating all issues
   from that week.

==============================================================
Current Description                                                                                                                                                            
                                                                                                                                                                                 
  The JIRA Monitor is an automated daily pipeline that ingests engineering issues from four HyCloud projects, processes them through an AI stack, and generates structured
  per-issue summaries stored in a shared database. The system automatically filters activity to relevant team members, reducing manual effort in tracking and reporting.         
                  
  ---                                                                                                                                                                            
  Future Plan — Formula
                                                                                                                                                                                 
  ┌─────────────────────────────┬─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
  │           Feature           │                                                                 Formula                                                                 │      
  ├─────────────────────────────┼─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ Leader-Specific Summary     │ Each active team leader receives a filtered weekly digest scoped to their domain; issue routing accuracy ≥ 90% validated bi-weekly      │
  ├─────────────────────────────┼─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ Interactive Query Interface │ Leaders can retrieve team summaries and blockers via natural language without manual dashboard access; ≥ 4 query capabilities available │      
  └─────────────────────────────┴─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘      
                                                                                                                                                                                 
  ---                                                                                                                                                                            
  Daily Maintenance — Formula
                                                                                                                                                                                 
  ┌──────────────────────┬────────────────────────────────────────────────────────────────────────┐
  │         Area         │                                Formula                                 │                                                                              
  ├──────────────────────┼────────────────────────────────────────────────────────────────────────┤
  │ Pipeline Reliability │ Daily run completes without manual intervention ≥ 95% of business days │
  ├──────────────────────┼────────────────────────────────────────────────────────────────────────┤
  │ Summary Freshness    │ Per-issue summary reflects latest activity within 24 hours of update   │
  ├──────────────────────┼────────────────────────────────────────────────────────────────────────┤                                                                              
  │ Profile Hygiene      │ Team roster and domain mappings reviewed and updated monthly           │
  └──────────────────────┴────────────────────────────────────────────────────────────────────────┘ 



===========================================================

Current Description                                                                                                                                                            
                                                                                                                                                                                 
  JIRA History RAG is a knowledge base system that consolidates all historical JIRA issues into a unified, queryable index. Built on a hierarchical retrieval structure (HiRAG)
  combined with an LLM-maintained knowledge layer, the system serves two purposes: enabling engineers and leaders to query past issue history in natural language, and providing 
  the daily summary pipeline with relevant historical context to produce more accurate and informed summaries.
                                                                                                                                                                                 
  ---             
  Future Plan — Formula

  ┌──────────────────────┬───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
  │       Feature        │                                                                      Formula                                                                      │
  ├──────────────────────┼───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ Historical Query     │ Engineers can retrieve relevant past issues, root causes, and resolutions via natural language; query returns accurate results for ≥ 80% of test  │
  │                      │ questions (evaluated bi-monthly)                                                                                                                  │
  ├──────────────────────┼───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤   
  │ Summary Augmentation │ Daily JIRA summaries enriched with historical context show measurable improvement in root cause accuracy vs. baseline; validated by bi-weekly     │
  │                      │ leader spot check ≥ 4/5                                                                                                                           │   
  ├──────────────────────┼───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ Knowledge Base       │ ≥ 95% of historical JIRA issues across all four projects indexed and queryable at launch                                                          │   
  │ Coverage             │                                                                                                                                                   │   
  └──────────────────────┴───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
                                                                                                                                                                                 
  ---             
  Daily Maintenance — Formula

  ┌────────────────────┬───────────────────────────────────────────────────────────────────────────────┐
  │        Area        │                                    Formula                                    │
  ├────────────────────┼───────────────────────────────────────────────────────────────────────────────┤
  │ Index Freshness    │ New issues added to the knowledge base within 24 hours of pipeline completion │
  ├────────────────────┼───────────────────────────────────────────────────────────────────────────────┤
  │ Query Availability │ System accessible during business hours with error rate ≤ 1%                  │                                                                         
  ├────────────────────┼───────────────────────────────────────────────────────────────────────────────┤                                                                         
  │ Knowledge Quality  │ Stale or conflicting entries reviewed and corrected on a monthly basis        │                                                                         
  └────────────────────┴───────────────────────────────────────────────────────────────────────────────┘                                                                         
   2. Optimization Plan & Timeline

  ┌──────────┬────────────────────────┬────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
  │ Timeline │        Feature         │                                                              Formula                                                               │
  ├──────────┼────────────────────────┼────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ Q3 2026  │ Historical Query       │ ≥ 95% of historical JIRA issues indexed at launch; query returns accurate results for ≥ 80% of test questions evaluated bi-monthly │
  ├──────────┼────────────────────────┼────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ Q4 2026  │ Summary Augmentation   │ Daily summaries enriched with historical context; improvement validated by leader spot check ≥ 4/5 bi-monthly                      │     
  ├──────────┼────────────────────────┼────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤     
  │ Q1 2027  │ HiRAG + LLM Wiki Layer │ Hierarchical knowledge structure established; knowledge base self-updates with each daily pipeline run without manual intervention │     
  └──────────┴────────────────────────┴────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘     
                  
                                                                                                                
                  

