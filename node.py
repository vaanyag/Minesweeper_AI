class Node:
    def __init__(self, docId, header_freq, body_freq, next = None):
        self.next = next
        self.docId = docId
        self.header_freq = header_freq
        self.body_freq = body_freq
        self.tf_idf_score = 0.0