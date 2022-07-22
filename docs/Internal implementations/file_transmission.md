# File transmission

This library allows sending and receiving files from telemetry.  
It is used to send to telemetry binary files that will be flashed on ECUs on the car.  
Some of the ECUs have the ability to update the firmware via CAN, so the telemetry receives the updated firmware and then flashes it to the correct ECU.

## Usage
Fist establish a connection between sender and receiver.  
Then instantiate a transaction object: ```FileTransfer::FileTransferManager::transaction_t;```  
There are two constructors, one for the sender and one for the receiver.  
Basically you must setup filename and destination path. And max_chunk_size (in bytes), a connection pointer, and a callback function.  
The callback function is used to notify the user about the events.
A callback example is:
~~~c++
file_transfer_callback callback = [](const int &id_, TRANSACTION_EVENT event){
    cout << "Transaction <" << id << "> event: " << TRANSACTION_EVENT_STRING[event] << endl;
};
~~~

The FileTransferManager class is used to manage the transactions.  
Instantiate a FileTransfer::FileTransferManager object. This can be global as it can handle multiple file transfers (both send and receive).  
~~~c++
FileTransfer::FileTransferManager ftm;
~~~
### Send
To start a file transfer for sending: Call FileTransfer::FileTransferManager::send().  
The send function handles all the sending process, it creates a thread and returns the id of the transaction.  
The sending thread can fill the queue with messages, but checks the queue rememaining size before sending, to avoid messages being dropped.  

Create a transaction object: ```FileTransfer::FileTransferManager::transaction_t;``` with the sender constructor:
~~~c++
FileTransfer::FileTransferManager::transaction_t transaction(
    "bin/telemetry", // filename
    "bin",           // destination path
    &conn,           // connection pointer
    "file_transfer", // topic
    4096,            // max_chunk_size in bytes
    callback         // callback function
);
~~~

Then simply to start sending the file:
~~~c++
int transaction_id = ftm.send(transaction);
if(transaction_id == -1){
    printf("Failed to send file\n\r");
}
~~~

The send sequence is:

- The transaction begin mesage is sent.

- File chunks are sent in order.

- The transaction end message is sent.  

**Transaction begin message**
~~~json
{
    "filename": "telemetry",
    "dest_path": "bin/",
    "total_chunks": 1000,
    "transaction_hash": 000
}
~~~
The transaction hash is done on filename and destination path, so two transactions can have the same hash.
Total chunks is the number of chunks that will be sent.

**Chunk message**
~~~json
{
    "chunk_n": 1,
    "chunk_total": 1,
    "transaction_hash": 000,
    "data": "chunk data"
}
~~~
**Transaction end message**
~~~json
{
    "transaction_hash": 000;
}
~~~

### Receive
A receiving transaction is defined by the topic_begin, from that message you can get the filename, destination path and total_chunks.  
When receiving topic_begin call FileTransfer::FileTransferManager::init_receive().  
Each time you receive a topic you can get the chunk_id and the data, and call FileTransfer::FileTransferManager::receive().
The transaction end is notified by the topic_end message.  
When the transaction is finished you can call FileTransfer::FileTransferManager::end_receive(). This function can return false if not all chunks were received.  


The receiver must handle the link beween the transaction hash and the transaction id.
A simple way is to use a hash table.
~~~c++
// key: transaction hash
// value: transaction id
std::unordered_map<int, int> transfers;
~~~

When receiving a transaction begin message, create a transaction object: ```FileTransfer::FileTransferManager::transaction_t;``` with the receiver constructor:
~~~c++
FileTransfer::FileTransferManager::transaction_t rcv_transaction(
    message.filename,
    message.dest_path,
    message.total_chunks,
    callback
);
~~~

Then initialize the receiving transaction and save the transaction id:
~~~c++
int id = ftm.init_receive(rcv_transaction);
if(id != -1){
    transfers.insert({message.transaction_hash,  id});
    printf("<%d> -> %s\n\r", message.transaction_hash, message.filename.c_str());
}else{
    printf("Failed to receive file %s\n\r", message.filename.c_str());
}
~~~

When receiving a chunk message, check if the transaction id is in the hash table.
If it is, then save the chunk:
~~~c++
auto it = transfers.find(chunk.transaction_hash);
if(it != transfers.end()){
    if(!ftm.receive(it->second, msg)){
        printf("FileTranferManager did not find transaction");
    }else{
        printf("<%d> received chunk %d of %d: %f\n", chunk.transaction_hash, chunk.chunk_n, chunk.chunk_total, (float)chunk.chunk_n / chunk.chunk_total);
    }
}else{
    printf("Transaction id unrecognized %d\n\r", chunk.transaction_hash);
}
~~~

When receiving a transaction end message, check if the transaction id is in the hash table.
If it is, then finish the transaction:
~~~c++
auto it = transfers.find(chunk.transaction_hash);
if(it != transfers.end()){
    if(ftm.end_receive(it->second)){
        transfers.erase(it);
        printf("<%d> -> End\n\r", chunk.transaction_hash);
    }else{
        printf("<%d> -> FAILED end receive\n\r", chunk.transaction_hash);
    }
}else{
    printf("End transaction failed\n\r");
}
~~~