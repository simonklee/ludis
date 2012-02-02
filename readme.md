ludis — an efficient and fast Redis protocol parser.

The goal of this project is to implement a memory efficient,
and fast Redis protocol parser in c. 

# Redis Protocol

    +----------+--------+--------+--------+
    |8-bit type|var data|16-bit terminator|
    +----------+--------+--------+--------+

Data size and protocol varies based on reply type.

## Types

    Binary          Dec  Hex     Character
    ========        ===  ===     =========
    00101011        43   2B      +
    00101101        45   2D      -
    00111010        58   3A      :
    00100100        36   24      $
    00101010        42   2A      *

## Terminator/separator

    Binary          Dec  Hex     Character
    ========        ===  ===     =========
    00001010        10   A       LF
    00001101        13   D       CR

## Data 

Data size varies based on type.

  +  status.
     
     Variable len data, termintated by CRLF

     Status reply `OK` ascii encoded

         +OK\r\n

     Status reply `OK` binary ascii encoded

         +--------+--------+--------+--------+--------+
         |00101011|01001111|01001011|00001101|00001010|
         +--------+--------+--------+--------+--------+
            type          data           terminator

  -  err.

     Variable len data, termintated by CRLF.
     See status type for example.

  :  integer.

     Variable len string repr an integer, terminated by
     CRLF. Can be as big as 64-bit signed int.

     Integer reply `50` ascii encoded

         :50\r\n

     Integer reply `50` binary ascii encoded

         +--------+--------+--------+--------+--------+
         |00111010|00110101|00110000|00001101|00001010|
         +--------+--------+--------+--------+--------+
            type          data           terminator

  $  bulk.

     Variable len data. The len of the payload is specified
     after the type, terminated by CRLF. The payload is then
     appended, terminated by CRLF. 

     The protocol extends to.
     
        +----------+--------+--------+--------+------+--------+--------+
        |8-bit type|data len|16-bit terminator| data |16-bit terminator|
        +----------+--------+--------+--------+------+--------+--------+

     Bulk reply `foo` ascii encoded

        $3\r\nfoo\r\n

     Bulk reply `foo` binary ascii encoded

        +--------+--------+--------+--------+
        |00100100|00110011|00001101|00001010|
        +--------+--------+--------+--------+
           type   data len     terminator

        +--------+--------+--------+--------+--------+
        |01100110|01101111|01101111|00001101|00001010|
        +--------+--------+--------+--------+--------+
                     data                terminator

  *  multi-bulk.

     Multiple bulk replies in a single transmition. Type and
     bulk count, terminated by CRLF, is prepended a single
     time before N replies. Bulk count denotes how many bulk
     reply follow. If bulk count is <= 0 nothing follows.

        +----------+----------+--------+--------+
        |8-bit type|bulk count|16-bit terminator| 
        +----------+----------+--------+--------+

     Multi-bulk reply `0` ascii encoded

        *0\r\n

     Multi-bulk reply `0` binary ascii encoded

        +--------+--------+--------+--------+
        |00100100|00110000|00001101|00001010|
        +--------+--------+--------+--------+
           type  bulk count    terminator
