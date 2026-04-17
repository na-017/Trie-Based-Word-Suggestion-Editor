<div align="center">
  **WELCOME TO OUR PROJECT**
</div>

This project is about a word suggestion editor.
We have used a Trie-Retrival System to store and access words which have been loaded from a dictionary(If you wish to use a different dictionary, move your preferred dictionary to the project directory and rename it as directory.txt). Currently we only support lowercase letters so please do make sure to type only lowercase letters. Also if your dictionary has any uppercase letters they will be converted to lowercase and any words with special characters will be ignored.

To create the final executable please run make on your terminal.

Upon running the final executable you will sent to a Terminal Interface containing menu page giving detailed statistics regarding the Trie used to store the dictionary word. We currently support 5 modes:

- F1: Insert Mode
    In this mode you will have the option to add any words of your liking or any words missed out in the directory to the Trie.

- F2: Delete Mode
    Here you will be able to remove any words your regard as not needed. Before removing a word you will be prompted to verify the change.

- F3: Search Mode
    This is the prefix search mode. Here upon typing a part of the word you will be recommended words with the typed part as a prefix.

- F4: Statistics Page:
    This page contains various statistics such as 
        - Total Word Count
        - Alphabetical Distribution of Words
        - Maximum Word Length
        - Total Memory Occupied by All the Nodes in the Trie

- F5: Word Suggestion System
    Here if your word is correct this system will recognize this and display if it is correct. Otherwise it will instead display the words closed to this which are correct according to the dictionary.

To exit the Terminal Interface please use Ctrl C.

Current Issues In the Project:
 - 

I hope you found our project useful.

THANK YOU!
