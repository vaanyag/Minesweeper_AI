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
#from node.py import Node
from collections import defaultdict



posting = dict()
num_docID = 1

for direc in pathlib.Path("/Users/samhithatarra/Desktop/Search-Engine/DEV").iterdir():
    for path in pathlib.Path(direc).iterdir():
        if path.is_file():
            total_token_count = 0
            tags_dict = defaultdict(list)
            find_lst = []
            unique_tokens = []
            current_file = open(path, "r")
            json_obj = json.load(current_file)
            content = json_obj['content']
            soup = BeautifulSoup(content,"lxml")
            
            header_lst = []
            
            tags_dict['p'] = [s for s in soup.findAll('p')]
            header_lst = [s for s in soup.findAll('h1')]
            for s in soup.findAll('h2'):
                header_lst.append(s)
            for s in soup.findAll('h3'):
                header_lst.append(s)
            for s in soup.findAll('title'):
                header_lst.append(s)
            for s in soup.findAll('b'):
                header_lst.append(s)
            for s in soup.findAll('strong'):
                header_lst.append(s)
            
            

            tags_dict['headers'] = header_lst
            


            for tag, words_lst in tags_dict.items():
                # 'p' = [words_lst(not tokenised)]
                token_lst = []
                for word in words_lst:
                    fin_text = ''.join(word.findAll(text=True))
                    fin_text = fin_text.lower()
                    nltk_tokens = nltk.word_tokenize(fin_text)

                    for token in nltk_tokens:
                        if len(token) >= 2:
                            token_lst.append(token)
                
                ps = PorterStemmer()
                token_freq_dict = defaultdict(int)
                for token in token_lst:
                    token=ps.stem(token)   # stemmed token 
                    if token not in unique_tokens:
                        unique_tokens.append(token)  # list of unique tokens for each json file 
                    token_freq_dict[token]+=1
                    total_token_count +=1
                tags_dict[tag] = token_freq_dict 
            
            #create the index and calculate tfidf and doc id
            # TF-IDF(headers) *0.7 + TF-IDF(body) *0.3 

            #tfidf calculation
            for token in unique_tokens:
                #need to initialize new node for linked list
                header_freq = 0
                body_freq = 0
                tf_score = 0
                if token in tags_dict['headers'][token]:
                    header_freq += tags_dict['headers'][token]
                if token in tags_dict['p'][token]:
                    body_freq += tags_dict['p'][token]
                header_freq = float(header_freq)/total_token_count
                body_freq = float(body_freq)/total_token_count
                    

            for k,v in tags_dict.items():
                print("***KEY***:", k, "VALUE:", v)
                
            current_file.close()

    #def calculate_tf():
        #pass
    
    # print(token_lst)
