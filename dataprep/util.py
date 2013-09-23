#!/usr/bin/env python
#encoding=utf8

def ids_to_string(ids):
    return [" ".join(map(lambda x: str(x)), id_pair) for id_pair in ids]
