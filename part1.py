# https://www.geeksforgeeks.org/python-stemming-words-with-nltk/ used for tokenizing and stemming

import DEV
import pathlib
import os
import json
import nltk
from nltk.stem import PorterStemmer 
from nltk.tokenize import word_tokenize
from bs4 import BeautifulSoup
import lxml


# ps = PorterStemmer() 
# # choose some words to be stemmed 
# words = ["program", "programs", "programer", "programing", "programers"] 
#   
# for w in words: 
#     print(w, " : ", ps.stem(w)) 

for direc in pathlib.Path("/Users/samhithatarra/Desktop/Search-Engine/DEV").iterdir():
    print(direc)
    for path in pathlib.Path(direc).iterdir():
        if path.is_file():
            find_lst = []
            token_lst = []
            current_file = open(path, "r")
            json_obj = json.load(current_file)
            content = json_obj['content']
            soup = BeautifulSoup(content,"lxml")
            find_lst.extend([s for s in soup.findAll('p')])
            find_lst.extend([s for s in soup.findAll('h1')])
            find_lst.extend([s for s in soup.findAll('h2')])
            find_lst.extend([s for s in soup.findAll('h3')])

            for s in find_lst:
                fin_text = ''.join(s.findAll(text=True))
                fin_text = fin_text.lower()
                nltk_tokens = nltk.word_tokenize(fin_text)
            
                for token in nltk_tokens:
                    if len(token) >= 2:
                        token_lst.append(token)
            ps = PorterStemmer()
            for token in token_lst:
                print(token, " : ", ps.stem(token))
            current_file.close()

    
    # print(token_lst)
