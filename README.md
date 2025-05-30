# Corpus Q&A Tool

This project is a Question and Answer (Q&A) tool designed to retrieve relevant paragraphs from a large corpus in response to user queries. It utilizes the BM25 ranking function to assess paragraph relevance and integrates with advanced language models to generate accurate answers.

## Features

- **Efficient Information Retrieval**: Implements the BM25 algorithm to rank paragraphs based on their relevance to user queries.
- **Advanced Answer Generation**: Integrates with language models like ChatGPT and Hugging Face to provide precise answers derived from the retrieved paragraphs.
- **Optimized Data Structures**: Employs efficient data structures, such as AVL trees and hashing techniques, to enhance storage and retrieval processes.

## Workflow

1. **Query Input**: The user submits a question or query.
2. **Paragraph Retrieval**: The tool processes the query and utilizes the BM25 algorithm to rank paragraphs within the corpus based on relevance.
3. **Answer Generation**: The top-ranked paragraphs are fed into language models (e.g., ChatGPT, Hugging Face) via API calls to generate accurate and contextually appropriate answers.
4. **Response Output**: The generated answer is presented to the user.

## File Structure

- **`qna_tool.cpp`**: Contains the main logic for handling user queries, orchestrating the retrieval and answer generation processes.
- **`qna_tool.h`**: Header file for `qna_tool.cpp`, declaring functions and data structures used in the Q&A tool.
- **`dict.cpp`**: Implements dictionary functionalities, including data structures like AVL trees and hashing mechanisms for efficient word storage and retrieval.
- **`dict.h`**: Header file for `dict.cpp`, defining the interfaces for dictionary operations.
- **`search.cpp`**: Implements the BM25 algorithm and other search-related functionalities to rank paragraphs based on query relevance.
- **`search.h`**: Header file for `search.cpp`, declaring search functions and related data structures.
