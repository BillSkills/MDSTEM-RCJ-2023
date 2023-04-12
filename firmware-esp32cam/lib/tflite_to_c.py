import os
from tensorflow.lite.python.util import convert_bytes_to_c_source

def convert_to_c(tflite_model,file_name):

    source_text, header_text = convert_bytes_to_c_source(tflite_model,  file_name)

    with  open(file_name + '.h',  'w')  as  file:

        file.write(header_text)

    with  open(file_name + '.cc',  'w')  as  file:

        file.write(source_text)

model = open(os.getcwd() + "/lib/tflite_models/lite-model_rosetta_dr_1.tflite", 'rb').read()
convert_to_c(model, os.getcwd() + "/lib/tflite_models/rosetta")