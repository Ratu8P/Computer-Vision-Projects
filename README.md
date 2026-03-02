[root@sjac pcie]# cat check_aer_read_only.sh
#!/usr/bin/bash

# trap ctrl-c and call ctrl_c()
trap ctrl_c INT

function ctrl_c() {
        printf "\n\nInterrupted at iteration: %d\n" $cnt
        printf "Total EP AER error count: %d\n" $ep_aer_err_cnt
        printf "Total EP UAER error count: %d\n" $ep_uaer_err_cnt
        printf "Total RC AER error count: %d\n" $rc_aer_err_cnt
        printf "Total RC UAER error count: %d\n" $rc_uaer_err_cnt
        printf "Total EP lane error count: %d\n" $lane_error_cnt
        printf "Total EP link down error count: %d\n" $link_down_err_cnt
        return 0
}

rate_decode () {
        if [ $1 == "gen5" ]; then
                return_val=4
        elif [ $1 == "gen4" ]; then
                return_val=3
        elif [ $1 == "gen3" ]; then
                return_val=2
        elif [ $1 == "gen2" ]; then
                return_val=1
        elif [ $1 == "gen1" ]; then
                return_val=0
        else
                return_val=10
                return
        fi
}

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 BDF rate_string count. Ex: $0 01:00.0 10000"
    echo "Usage: Where BDF is in the form of xx:xx.x "
    return
fi

Background
In our daily work, we often face challenges due to time zone differences, high workload, and the complexity of issues tracked in JIRA. Imagine a scenario: a customer from China reports an urgent issue, and our local Field Application Engineer (FAE) logs it in the JIRA system during their daytime. By the time our team begins reviewing the JIRA, more than 12 hours have passed, and our typical feedback request—like needing more debugging information—leads to further delays. This turnaround time wastes valuable debugging hours and prolongs resolution. The lack of real-time responses and extended waiting periods adds significant complexity, hindering timely action and creating a backlog of unresolved issues. Such delays in resolution can ultimately impact customer satisfaction and lead to further escalations.

Consider another real case: a recent critical issue involved over 500 comments on the JIRA ticket, alongside 10 different email threads, and more than 20 versions of firmware built and tested to address the problem. Each of these elements added layers of complexity, requiring extensive coordination and handling different sets of data, test outcomes, and action items. Engineers spend significant time not only interpreting and managing the technical aspects but also navigating through redundant and sometimes contradictory information. This overwhelming amount of data often leads to confusion, missed updates, and difficulty in maintaining focus on key progress points, with valuable time being wasted on tracking actions instead of solving the root cause. Imagine if an engineer could summarize daily updates, monitor action item progress, and generate concise reports—this would drastically reduce the administrative burden and allow engineers to focus on solving problems rather than organizing information.

Another complex scenario involves understanding and managing all the background data related to an issue. Imagine an engineer who could read and remember every design document, Confluence page, firmware change list, and debug history—working tirelessly around the clock, in multiple languages, and never forgetting a single detail. Consolidating insights from thousands of pages of documentation, keeping track of firmware changes, and understanding complex configurations is a monumental task. Such challenges necessitate a streamlined system that can provide quick, organized, and relevant information. This is where AI steps in, automating these arduous tasks and ensuring engineers have the insights they need at their fingertips.

Introducing "JIRA Monitor"
"Rome wasn't built in a day." While our vision is ambitious, we believe in taking practical, focused steps. Enter JIRA Monitor—a software solution developed to streamline the process of JIRA ticket monitoring. JIRA Monitor helps teams effectively track updates, debugging directions, and test results, eliminating the confusion and inefficiencies often associated with complex projects. By presenting the right information at the right time, JIRA Monitor brings clarity to chaos, ensuring that key details are easily manageable and actionable.

Implementation Overview
JIRA Monitor consists of several core components:

1. JIRA Collection/Download
JIRA data is collected into a local data pool using JIRA keywords or JIRA Query Language (JQL). Users can define criteria like searching by specific groups or downloading comments made by selected people within the past week.

JIRA Download Example

2. Context Processing
The system converts complex tables, images, and logs into a format that's suitable for large language models (LLMs). Due to LLM token size limitations, JIRA Monitor compresses context effectively, preserving key information and debugging details.

Context Processing Example

3. Key Information Extraction
Using a Graph-RAG approach, the system extracts key debugging nodes and their relationships by leveraging both graph structures and Retrieval-Augmented Generation (RAG) techniques. Graph-RAG enables the system to represent the debugging process as a network of interconnected nodes, where each node represents a critical piece of information, such as test results, firmware versions, or specific debugging steps. Relationships between these nodes are mapped to illustrate dependencies, causal links, and the flow of information throughout the debugging process. This graph-based approach allows for a deeper understanding of complex issues by visualizing how different components interact and evolve over time. This functionality is built on the Light-RAG library, tailored for our specific debugging needs, focusing on areas like PCIe and NVMe problems, and helps to improve the precision and efficiency of extracting and organizing relevant debugging information.

Key Information Extraction Example

4. Enhanced Query Generation
From user queries, the system extracts high-level and detailed keywords, which are used to rank relevant information efficiently. This component leverages the Graph-RAG concept, combining graph structures with Retrieval-Augmented Generation (RAG) to enhance the accuracy of keyword extraction and information retrieval. The system builds a graph-based representation of the debugging data, mapping relationships between nodes (i.e., key pieces of information), and utilizes this structure to provide context-aware answers. This component also uses the **Light-RAG** library for streamlined implementation and performance optimization.

Enhanced Query Generation Example - 1

Enhanced Query Generation Example - 2

Long-Term Vision
PAE AI Development is an ongoing journey, and JIRA Monitor is only the beginning. Future growth will require more engineers and stronger hardware. We plan to expand data collection to include emails, Confluence pages, internal design documents, and specifications.

A user-friendly interface is also part of our roadmap, allowing easy access to the benefits of JIRA Monitor. Ultimately, we aim to refine our data collection mechanism to support fine-tuning our own AI model, leveraging high-performance hardware to further enhance capabilities.

Summary
Our PAE AI Development project, through the JIRA Monitor software, aims to simplify and improve user interaction with JIRA. By providing automated summaries, efficient tracking, and streamlined question-answering, we reduce the time spent on manual monitoring and ensure key information is always accessible. During the recent PAE team AI development discussions, we identified that tools like JIRA Monitor can significantly enhance our efficiency, especially in handling complex debugging workflows and managing large volumes of information across multiple platforms.
