/*
 * DaaS-IoT 2019, 2025 (@) Sebyone Srl
 *
 * File: daas.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Disclaimer of Warrant
 * Covered Software is provided under this License on an "as is" basis, without warranty of any kind, either
 * expressed, implied, or statutory, including, without limitation, warranties that the Covered  Software is
 * free of defects, merchantable, fit for a particular purpose or non-infringing.
 * The entire risk as to the quality and performance of the Covered Software is with You.  Should any Covered
 * Software prove defective in any respect, You (not any Contributor) assume the cost of any necessary
 * servicing, repair, or correction.
 * This disclaimer of warranty constitutes an essential part of this License.  No use of any Covered Software
 * is authorized under this License except under this disclaimer.
 *
 * Limitation of Liability
 * Under no circumstances and under no legal theory, whether tort (including negligence), contract, or otherwise,
 * shall any Contributor, or anyone who distributes Covered Software as permitted above, be liable to You for
 * any direct, indirect, special, incidental, or consequential damages of any character including, without
 * limitation, damages for lost profits, loss of goodwill, work stoppage, computer failure or malfunction,
 * or any and all other commercial damages or losses, even if such party shall have been informed of the
 * possibility of such damages.  This limitation of liability shall not apply to liability for death or personal
 * injury resulting from such party's negligence to the extent applicable law prohibits such limitation.
 * Some jurisdictions do not allow the exclusion or limitation of incidental or consequential damages, so this
 * exclusion and limitation may not apply to You.
 *
 * Contributors:
 * plogiacco@smartlab.it - initial design, implementation and documentation
 * sebastiano.meduri@gmail.com  - initial design, implementation and documentation
 * l.grillo@sebyone.com  - implementation and documentation
 *
 */

#ifndef DAAS_H
#define DAAS_H

#include "daas_types.hpp"

/* ----------------------------------------------------------------------------------------------------------- */

class DaasAPI
{
public:
    DaasAPI();                                    
    DaasAPI(IDaasApiEvent *);                     
    DaasAPI(IDaasApiEvent *, const char *lhver_); 
    ~DaasAPI();

    //////////////////////////////////////////////////////////////////////
    ////  G E N E R A L S                                             ////
    //////////////////////////////////////////////////////////////////////

    const char *listAvailableDrivers();           // Returns drivers list: 2.INET4;3.UART;4.MQTT
    const char *getVersion();                     // returns daas-version
    const char *getBuildInfo();                     // returns local daas-stack details
    
    /**
        End
        - Releases resources and deactivates the node.
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t doEnd();                         // releases resources and deactivates node
   
    /**
        Reset
        - Resets the local node and clears all resources.
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t doReset();                       // reset resources and restarts services
    
    /**
        Initialize
        - sid_: SID of the local node
        - din_: DIN of the local node
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t doInit(din_t sid_, din_t din_);  // initializes services and resources (Real-Time or Multi-Threading, release dependent)
    
    /**
        Perform
        - mode: PERFORM_CORE_NO_THREAD for real-time mode, PERFORM_CORE_THREAD for multi-threading mode
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t doPerform(performs_mode_t mode); // perform node's task ( in RT mode needs to be called cyclically)
    
    /**
        Enable Driver
        - driver_id: the communication technology to enable (e.g., _LINK_INET4, _LINK_UART, _LINK_MQTT5)
        - local_uri: the physical address of the local node
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t enableDriver(link_t driver_id, const char *local_uri); // Configure driver for network technology (links)

    /**
        Get Status
        - Returns the status of the local node.
        - This includes hardware version, linked channels, synchronization status, security policy, and more.
    */
    nodestate_t getStatus(); // returns local node's instance status
     
    /*
        Backup Configuration
        - Saves the current configuration to the specified storage interface.
        - Returns true if the backup was successful, false otherwise.
    */
    bool storeConfiguration(IDepot* storage_interface);

    /*
        Load Configuration
        - Loads the configuration from the specified storage interface.
        - Returns true if the configuration was loaded successfully, false otherwise.
    */
    bool loadConfiguration(IDepot* storage_interface);
    
    /*
        Do Statistics Reset
        - Resets the system's statistics data.
        - Returns true if the reset was successful, false otherwise.
    */
    bool doStatisticsReset();

    /*
        Get System Statistics
        - label: the system code to get statistics for (e.g., _cor_dme_sended)
        - Returns the system statistics for the given label.
    */
    uint64_t getSystemStatistics(syscode_t label); 
   
    /* Mapping      -------------------------------------------------------------------------------------------- */
    
    /*
        Map: Maps a node to the local instance
        - din: DIN of the node to map
        - link_: the communication technology to use (e.g., _LINK_INET4, _LINK_UART, _LINK_MQTT5)
        - suri: the physical address of the node (e.g., "192.168.1.1:2020")
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t map(din_t din);                                                   // adds new node to local instance
    daas_error_t map(din_t din, link_t link_, const char *suri);                   // adds node-identifier and related physical address ( link: 1="INET4", 2="UART", 3="MQTT5")
    daas_error_t map(din_t din, link_t link_, const char *suri, const char *skey); // adds node-identifier and related physical address ( link: 1="INET4", 2="UART", 3="MQTT5")
    
    /*
        Remove: Removes a node from the local instance
        - din: DIN of the node to remove from the local instance
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t remove(din_t din);

    /* Availability -------------------------------------------------------------------------------------------- */
    
    /*
        List Nodes
        - Returns a list of known nodes (din_t) in the local instance.
    */
    dinlist_t listNodes();                       // Returns map entries  (knows nodes) ( din1, din2, )
    
    /*
        Locate
        - din_: DIN of the node to locate to be able to communicate with it.
        - It starts a process to locate the node if it is not inside the known table.
        - Returns ERROR_NONE if the node is known, or an error code if it is not.
    */
    daas_error_t locate(din_t din);
    
    /*
        Send local node's status to remote node (din)
        - din: DIN of the remote node to send status to
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t send_status(din_t din); // Send local status to remote node (din)
    
    /*
        Status
        - din: DIN of the remote node to get status from
        - Returns the nodestate_t of the remote node.
    */
    const nodestate_t& status(din_t din);

    /*
        Fetch
        - din: DIN of the remote node to fetch
        - opts: options for fetching (e.g., 0 for default, 1 for detailed)
        - Returns the nodestate_t of the remote node after fetching.
        - This function tries to connect to the remote node and update its status.
    */
    const nodestate_t& fetch(din_t din, uint16_t opts);

    /*
        Get Synced Timestamp
        - This is the time with ATS correction to be able to communicate inside the net.
        - Returns the synced timestamp of the local node.
    */
    uint64_t getSyncedTimestamp();

    /* Security     -------------------------------------------------------------------------------------------- */
    
    /*
        Unlock
        - din: DIN of the remote node to unlock
        - skey: the security key to set
        - Returns the nodestate_t of the remote node after unlocking.
    */
    const nodestate_t& unlock(din_t din, const char *skey); 
    
    /*
        Set SKEY and security policy for local node
        - skey: the security key to set
        - policy_: the security policy to set
        - Returns the nodestate_t of the local node after setting the security key and policy.
    */
    const nodestate_t& lock(const char *skey, unsigned policy_);

    /* Synchronize  -------------------------------------------------------------------------------------------- */
    
    /*
        Set the local system time on remote node <din> and synchronize ATS
        - timezone: the timezone offset in seconds
        - Returns the nodestate_t of the remote node after synchronization.
    */
    const nodestate_t& syncNode(din_t din, unsigned timezone);  

    /*
        Set the local system time on remote node <din> and synchronize ATS
        - bubble_time: time in milliseconds to wait for the synchronization to complete
        - Returns the nodestate_t of the remote node after synchronization.
    */
    const nodestate_t& syncNet(din_t din, unsigned bubble_time);

    /*
        Set the maximum error allowed for ATS synchronization
        - error: the maximum error in milliseconds
    */
    void setATSMaxError(int32_t error); // ATS

    /* Exchange     -------------------------------------------------------------------------------------------- */
    
    /*
        Use
        - din: DIN of the remote node to use
        - Returns true if the RT session was successfully started, false otherwise. (OPEN CONNECTION!!!!)
    */
    bool use(din_t din);                                                    

    /*
        End
        - din: DIN of the remote node to end the RT session with
        - Returns true if the RT session was successfully ended, false otherwise.
    */
    bool end(din_t din);

    /*
        Send
        - din: DIN of the remote node to send data to
        - outbound: pointer to the data to send
        - size: size of the data to send
        - Returns the size of data sent.
    */
    unsigned send(din_t din, unsigned char *outbound, unsigned size);

    /*
        Received
        - din: DIN of the remote node to receive data from
        - Returns the size of data received.
    */
    unsigned received(din_t din);


    /*
        Receive
        - din: DIN of the remote node to receive data from
        - inbound: reference to a variable that will hold the received data
        - max_size: maximum size of data to receive
        - Returns the size of data received.
    */
    unsigned receive(din_t din, unsigned char &inbound, unsigned max_size);

    /* Transfer     -------------------------------------------------------------------------------------------- */
    /*
        List Typesets
        - Returns a reference to the list of user-defined typesets.
        - The list is of type tsetlist_t, which is a Vector of typeset
    */
    tsetlist_t &listTypesets();
   
    /*
        Pull
        - din: DIN of the remote node to pull data from
        - inboundDDO: pointer to a DDO pointer that will hold the pulled DDO
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t pull(din_t din, DDO **inboundDDO);

    /*
        Push
        - din: DIN of the remote node to send data to
        - outboundDDO: pointer to the DDO to send
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t push(din_t din, DDO *outboundDDO);

    /*
        Available pull
        - din: DIN of the remote node to pull data from
        - count: reference to a variable that will hold the number of available DDOs
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t availablesPull(din_t din, uint32_t &count);

    /*
        Adds user-defined typeset
        - typeset_code: the code of the typeset to add
        - typeset_size: the size of the typeset in bytes
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t addTypeset(const uint16_t typeset_code, const uint16_t typeset_size);
    
    
    /* TEST */

    /*
        Frisbee
        - din: DIN of the remote node to ping
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t frisbee(din_t din);               

    /*
        Frisbee ICMP
        - din: DIN of the remote node to ping
        - timeout: maximum time to wait for a reply (in milliseconds)
        - retry: number of retries if no reply is received
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t frisbee_icmp(din_t din, uint32_t timeout, uint32_t retry); 

    /*
        Frisbee performance test
        - din: DIN of the remote node to ping
        - sender_pkt_total: total number of packets to send
        - block_size: size of each packet in bytes
        - sender_trip_period: time period between each packet sent (in milliseconds)
        - Returns ERROR_NONE on success, or an error code on failure.
    */
    daas_error_t frisbee_dperf(din_t din, uint32_t sender_pkt_total = 10, uint32_t block_size = 1024*1024, uint32_t sender_trip_period = 0); 
    
    /*
        Frisbee performance result
        - sender_first_timestamp: timestamp of the first packet sent by the sender
        - local_end_timestamp: timestamp of the last packet received by the sender
        - remote_first_timestamp: timestamp of the first packet received by the receiver
        - remote_last_timestamp: timestamp of the last packet received by the receiver
        - remote_pkt_counter: number of packets received by the receiver
        - remote_data_counter: total data received by the receiver in bytes
    */
    dperf_info_result get_frisbee_dperf_result();
};

#endif // DAASIOT_H
