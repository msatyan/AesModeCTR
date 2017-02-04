## Copyright 2017 Sathyanesh Krishnan

## Licensed under the Apache License, Version 2.0 (the "License")


# AesEncriptionModes

This example (AesModeCTR) uses AES256 with CTR mode for encryption and decryption. Then the data's length does not need to be a multiple of AES's block size.

A pure AES encryption (ECB mode) will produce identical ciphertext blocks for identical plaintext blocks; that may not hide data patterns well. 
Then it is compromising message confidentiality, 
then it not advisable for any seriously cryptographic protocols at all. 
  
This limitation can be overcome by choosing appropriate encryption modes that best suit for your work. 
By choosing Counter (CTR) mode we can achieve parallelization of Encryption and Decryption operation also Random read access too. 
In addition to encryption key, be careful not to compromise the confidentiality of initialization vector (IV) too while using CTR mode.

