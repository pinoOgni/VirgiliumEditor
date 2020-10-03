//
// Created by alex on 10/08/20.
//

#ifndef VIRGILIUM_CONSTANTS_H
#define VIRGILIUM_CONSTANTS_H

#define _int qint64
#define FILE_IDENTIFIER 993
#define ConnectionWaitingTime 5000 // in millisecondi
#define Q_DATA_STREAM_VERSION QDataStream::Qt_5_1
#define LISTENING_PORT 9999

/*Lista di costanti che discrimina i segnali e quello che deve fare l'applicazione:
 * */

#define BASIC_MESSAGE 1
#define CRDT_MESSAGE 2
#define SITE_ID_ASSIGNMENT 3
#define SYMBOL_INSERT_OR_ERASE 4
#define CLIENT_CONNECTED 5
#define LOGIN_OK 6
#define LOGIN_KO 7
#define SIGNUP_OK 8
#define SIGNUP_KO 9
#define LOGOUT 10
#define CURSOR_CHANGED 11
#define LOAD_REQUEST 12
#define LOAD_RESPONSE 13
#define SAVE 14
#define DELETE_ACTIVE 15

//list of constants for interactions with the database
#define LOGIN  701
#define SIGNUP 702
#define GET_FILES_OWNER 703
#define GET_INFO_USER 704
#define GET_FILES_COLLABORATOR 705
#define RENAME_FILE 706
#define DELETE_FILE 707
#define CHANGE_PASSWORD 708
#define NEW_FILE 709
#define ADD_COLLABORATOR 710
#define REMOVE_COLLABORATOR 711
#define GET_ALL_DATA 712
#define UNSUBSCRIBE 713

#define GET_FILES_OWNER_OK 731
#define GET_FILES_OWNER_KO 732
#define GET_FILES_COLLABORATOR_OK 733
#define GET_FILES_COLLABORATOR_KO 734
#define GET_ALL_DATA_OK 735
#define RENAME_FILE_OK 736
#define RENAME_FILE_KO 737
#define DELETE_FILE_OK 738
#define DELETE_FILE_KO 739
#define NEW_FILE_OK 740
#define NEW_FILE_KO 741
#define CHANGE_PASSWORD_OK 742
#define CHANGE_PASSWORD_KO 743
#define ADD_COLLABORATOR_OK 744
#define ADD_COLLABORATOR_KO 745
#define REMOVE_COLLABORATOR_OK 746
#define REMOVE_COLLABORATOR_KO 747
#define UNSUBSCRIBE_OK 748
#define UNSUBSCRIBE_KO 749
#define CREATE_INVITE 750
#define INVITE_CREATED 751
#define REQUEST_TO_COLLABORATE 752
#define REQUEST_TO_COLLABORATE_OK 753
#define REQUEST_TO_COLLABORATE_KO 754

//true --> ricrea i db vergini
//false --> usa il DB con i dati che ci sono
#define TESTDB true
#define VIRGILIUM_STORAGE "/VIRGILIUM_STORAGE/"

#endif //VIRGILIUM_CONSTANTS_H
