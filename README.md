# DES Linear Cryptoanalysis
The repository contains DES encryption/decryption, and DES linear cryptoanalysis.

The algorithm is reproduced referring to "The First Experimental Cryptanalysis of the Data Encryption Standard" and "Linear Cryptanalysis Method for DES Cipher" by Mitsuru Matsui. 

For further instructions on the algorithm, please refer to the papers that I have mentioned.

Instructions of the main panel:
![image](https://github.com/YFMao15/DESLinearCryptoanalysis/blob/master/Capture.PNG)

DES implementation part:

Select button is for pick up the designated file in UI interface, or typing the path into the combobox is also acceptable.

After choosing the input file, click on Read File button to read the files into memory.

To encrypt/decrypt file, click on the Encrypt/Decrypt File button to start the process.

The new file will be named as 'XXX_new", which will be saved into the folder of saving directory. 


DES cryptoanalysis part:

File selection is similar to the content above.

After reading the files, click Linear Attack button to start linear analysis.

The left edit box will output the linearity of all S-boxes, while the right one will display the key bits discovered from linear analysis.
