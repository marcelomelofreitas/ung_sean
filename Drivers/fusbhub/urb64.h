/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

        USB.H

Abstract:

   structures and APIs for USB drivers.

Environment:

    Kernel & user mode
Revision History:

    09-29-95 : created
    02-10-04 : Updated to include header versioning

--*/
#ifndef URB_64_HEADER
#define URB_64_HEADER
#pragma warning( disable : 4200 )

extern "C" {
#pragma pack(push, 8)
#include <ntddk.h>
#pragma pack(pop)

typedef ULONG64 USBD_PIPE_HANDLE_64;
typedef ULONG64 USBD_CONFIGURATION_HANDLE_64;
typedef ULONG64 USBD_INTERFACE_HANDLE_64;

struct _URB_HEADER_64 {
    //
    // Fields filled in by client driver
    //
    USHORT Length;
    USHORT Function;
    USBD_STATUS Status;
    //
    // Fields used only by USBD
    //
    ULONG64 UsbdDeviceHandle; // device handle assigned to this device
                            // by USBD
    ULONG UsbdFlags;        // flags field reserved for USBD use.
};

typedef enum _USBD_PIPE_TYPE_64 {
    UsbdPipeTypeControl_64,
    UsbdPipeTypeIsochronous_64,
    UsbdPipeTypeBulk_64,
    UsbdPipeTypeInterrupt_64
} USBD_PIPE_TYPE_64;

typedef struct _USBD_PIPE_INFORMATION_64 {
    //
    // OUTPUT
    // These fields are filled in by USBD
    //
    USHORT MaximumPacketSize;  // Maximum packet size for this pipe
    UCHAR EndpointAddress;     // 8 bit USB endpoint address (includes direction)
                               // taken from endpoint descriptor
    UCHAR Interval;            // Polling interval in ms if interrupt pipe

    USBD_PIPE_TYPE_64	PipeType;   // PipeType identifies type of transfer valid for this pipe
    USBD_PIPE_HANDLE_64	PipeHandle;

    //
    // INPUT
    // These fields are filled in by the client driver
    //
    ULONG MaximumTransferSize; // Maximum size for a single request
                               // in bytes.
    ULONG PipeFlags;
} USBD_PIPE_INFORMATION_64, *PUSBD_PIPE_INFORMATION_64;


typedef struct _USBD_INTERFACE_INFORMATION_64 {
    USHORT Length;       // Length of this structure, including
                         // all pipe information structures that
                         // follow.
    //
    // INPUT
    //
    // Interface number and Alternate setting this
    // structure is associated with
    //
    UCHAR InterfaceNumber;
    UCHAR AlternateSetting;

    //
    // OUTPUT
    // These fields are filled in by USBD
    //
    UCHAR Class;
    UCHAR SubClass;
    UCHAR Protocol;
    UCHAR Reserved;

    USBD_INTERFACE_HANDLE_64 InterfaceHandle;
    ULONG NumberOfPipes;

    //
    // INPUT/OUPUT
    // see PIPE_INFORMATION

    USBD_PIPE_INFORMATION_64 Pipes[1];
} USBD_INTERFACE_INFORMATION_64, *PUSBD_INTERFACE_INFORMATION_64;

struct _URB_SELECT_INTERFACE_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    USBD_CONFIGURATION_HANDLE_64 ConfigurationHandle;

    // client must input AlternateSetting & Interface Number
    // class driver returns interface and handle
    // for new alternate setting
    USBD_INTERFACE_INFORMATION_64 Interface;
};

typedef struct _USB_CONFIGURATION_DESCRIPTOR_64 {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT wTotalLength;
    UCHAR bNumInterfaces;
    UCHAR bConfigurationValue;
    UCHAR iConfiguration;
    UCHAR bmAttributes;
    UCHAR MaxPower;
} USB_CONFIGURATION_DESCRIPTOR_64, *PUSB_CONFIGURATION_DESCRIPTOR_64;

struct _URB_SELECT_CONFIGURATION_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    // NULL indicates to set the device
    // to the 'unconfigured' state
    // ie set to configuration 0
    ULONG64 ConfigurationDescriptor;
    USBD_CONFIGURATION_HANDLE_64 ConfigurationHandle;
    USBD_INTERFACE_INFORMATION_64 Interface;
};

struct _URB_PIPE_REQUEST_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    USBD_PIPE_HANDLE_64 PipeHandle;
    ULONG Reserved;
};

struct _URB_FRAME_LENGTH_CONTROL_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
};

struct _URB_GET_FRAME_LENGTH_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    ULONG FrameLength;
    ULONG FrameNumber;
};

struct _URB_SET_FRAME_LENGTH_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    LONG FrameLengthDelta;
};

struct _URB_GET_CURRENT_FRAME_NUMBER_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    ULONG FrameNumber;
};

struct _URB_HCD_AREA_64 {
    ULONG64 Reserved8[8];
};

struct _URB_CONTROL_TRANSFER_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    USBD_PIPE_HANDLE_64 PipeHandle;
    ULONG TransferFlags;
    ULONG TransferBufferLength;
    ULONG64 TransferBuffer;
    ULONG64 TransferBufferMDL;             // *optional*
    ULONG64 UrbLink;               // *reserved MBZ*
    struct _URB_HCD_AREA_64 hca;           // fields for HCD use
    UCHAR SetupPacket[8];
};

struct _URB_BULK_OR_INTERRUPT_TRANSFER_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    USBD_PIPE_HANDLE_64 PipeHandle;
    ULONG TransferFlags;                // note: the direction bit will be set by USBD
    ULONG TransferBufferLength;
    ULONG64 TransferBuffer;
    ULONG64 TransferBufferMDL;             // *optional*
    ULONG64 UrbLink;               // *optional* link to next urb request
                                        // if this is a chain of commands
    struct _URB_HCD_AREA_64 hca;           // fields for HCD use
};

struct _URB_CONTROL_TRANSFER_EX_64 {
    struct _URB_HEADER_64 Hdr;
    USBD_PIPE_HANDLE_64 PipeHandle;
    ULONG TransferFlags;
    ULONG TransferBufferLength;
    ULONG64 TransferBuffer;
    ULONG64 TransferBufferMDL;             // *optional*
    ULONG Timeout;                      // *optional* timeout in milliseconds
                                        // for this request, 0 = no timeout
                                        // if this is a chain of commands
    ULONG Pad;

	struct _URB_HCD_AREA_64 hca;           // fields for HCD use
    UCHAR SetupPacket[8];
};

typedef enum _USB_IO_PRIORITY_64  {

    UsbIoPriorty_Normal_64 = 0,
    UsbIoPriority_High_64 = 8,
    UsbIoPriority_VeryHigh_64 = 16

} USB_IO_PRIORITY_64  ;


typedef struct _USB_IO_PARAMETERS_64 {

    /* iomap and schedule priority for pipe */
    USB_IO_PRIORITY_64 UsbIoPriority;    
   
    /* max-irp, number of irps that can complete per frame */
    ULONG UsbMaxPendingIrps;

    /* These fields are read-only */
    /* This is the same value returned in the pipe_info structure and is the largest request the 
       controller driver can handle */
    ULONG UsbMaxControllerTransfer;

} USB_IO_PARAMETERS_64, *PUSB_IO_PARAMETERS_64;

struct _URB_PIPE_IO_POLICY_64 {
    
    struct _URB_HEADER_64 Hdr;
    /* NULL indicates default pipe */
    USBD_PIPE_HANDLE_64 PipeHandle; 
    USB_IO_PARAMETERS_64 UsbIoParamters;
  
};

struct _URB_CONTROL_DESCRIPTOR_REQUEST_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    ULONG64 Reserved;
    ULONG Reserved0;
    ULONG TransferBufferLength;
    ULONG64 TransferBuffer;
    ULONG64 TransferBufferMDL;             // *optional*
    ULONG64 UrbLink;               // *reserved MBZ*
    struct _URB_HCD_AREA_64 hca;           // fields for HCD use
    USHORT Reserved1;
    UCHAR Index;
    UCHAR DescriptorType;
    USHORT LanguageId;
    USHORT Reserved2;
};

struct _URB_CONTROL_GET_STATUS_REQUEST_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    ULONG64 Reserved;
    ULONG Reserved0;
    ULONG TransferBufferLength;
    ULONG64 TransferBuffer;
    ULONG64 TransferBufferMDL;             // *optional*
    ULONG64 UrbLink;               // *reserved MBZ*
    struct _URB_HCD_AREA_64 hca;           // fields for HCD use
    UCHAR Reserved1[4];
    USHORT Index;                       // zero, interface or endpoint
    USHORT Reserved2;
};

struct _URB_CONTROL_FEATURE_REQUEST_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    ULONG64 Reserved;
    ULONG Reserved2;
    ULONG Reserved3;
    ULONG64 Reserved4;
    ULONG64 Reserved5;
    ULONG64 UrbLink;               // *reserved MBZ*
    struct _URB_HCD_AREA_64 hca;           // fields for HCD use
    USHORT Reserved0;
    USHORT FeatureSelector;
    USHORT Index;                       // zero, interface or endpoint
    USHORT Reserved1;
};

struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    ULONG64 Reserved;
    ULONG TransferFlags;
    ULONG TransferBufferLength;
    ULONG64 TransferBuffer;
    ULONG64 TransferBufferMDL;             // *optional*
    ULONG64 UrbLink;               // *reserved MBZ*
    struct _URB_HCD_AREA_64 hca;           // fields for HCD use
    UCHAR RequestTypeReservedBits;
    UCHAR Request;
    USHORT Value;
    USHORT Index;
    USHORT Reserved1;
};

struct _URB_CONTROL_GET_INTERFACE_REQUEST_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    ULONG64 Reserved;
    ULONG Reserved0;
    ULONG TransferBufferLength;
    ULONG64 TransferBuffer;
    ULONG64 TransferBufferMDL;             // *optional*
    ULONG64 UrbLink;               // *reserved MBZ*
    struct _URB_HCD_AREA_64 hca;           // fields for HCD use
    UCHAR Reserved1[4];
    USHORT Interface;
    USHORT Reserved2;
};

struct _URB_CONTROL_GET_CONFIGURATION_REQUEST_64 {
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    ULONG64 Reserved;
    ULONG Reserved0;
    ULONG TransferBufferLength;
    ULONG64 TransferBuffer;
    ULONG64 TransferBufferMDL;             // *optional*
    ULONG64 UrbLink;               // *resrved MBZ*
    struct _URB_HCD_AREA_64 hca;           // fields for HCD use
    UCHAR Reserved1[8];
};

struct _URB_OS_FEATURE_DESCRIPTOR_REQUEST_64 {
    struct _URB_HEADER_64 Hdr;  // function code indicates get or set.
    ULONG64 Reserved;
    ULONG Reserved0;
    ULONG TransferBufferLength;
    ULONG64 TransferBuffer;
    ULONG64 TransferBufferMDL;             // *optional*
    ULONG64 UrbLink;               // *optional* link to next urb request
                                        // if this is a chain of commands
    struct _URB_HCD_AREA_64 hca;           // fields for HCD use
    UCHAR   Recipient:5;                // Recipient {Device,Interface,Endpoint}
    UCHAR   Reserved1:3;
    UCHAR   Reserved2;
    UCHAR   InterfaceNumber;            // wValue - high byte
    UCHAR   MS_PageIndex;               // wValue - low byte
    USHORT  MS_FeatureDescriptorIndex;  // wIndex field
    USHORT  Reserved3;
};

typedef struct _USBD_ISO_PACKET_DESCRIPTOR_64 {
    ULONG Offset;       // INPUT Offset of the packet from the begining of the
                        // buffer.

    ULONG Length;       // OUTPUT length of data received (for in).
                        // OUTPUT 0 for OUT.
    USBD_STATUS Status; // status code for this packet.
} USBD_ISO_PACKET_DESCRIPTOR_64, *PUSBD_ISO_PACKET_DESCRIPTOR_64;

struct _URB_ISOCH_TRANSFER_64 {
    //
    // This block is the same as CommonTransfer
    //
    struct _URB_HEADER_64 Hdr;                 // function code indicates get or set.
    USBD_PIPE_HANDLE_64 PipeHandle;
    ULONG TransferFlags;
    ULONG TransferBufferLength;
    ULONG64 TransferBuffer;
    ULONG64 TransferBufferMDL;             // *optional*
    ULONG64 UrbLink;               // *optional* link to next urb request
                                        // if this is a chain of commands
    struct _URB_HCD_AREA_64 hca;           // fields for HCD use

    //
    // this block contains transfer fields
    // specific to isochronous transfers
    //

    // 32 bit frame number to begin this transfer on, must be within 1000
    // frames of the current USB frame or an error is returned.

    // START_ISO_TRANSFER_ASAP flag in transferFlags:
    // If this flag is set and no transfers have been submitted
    // for the pipe then the transfer will begin on the next frame
    // and StartFrame will be updated with the frame number the transfer
    // was started on.
    // If this flag is set and the pipe has active transfers then
    // the transfer will be queued to begin on the frame after the
    // last transfer queued is completed.
    //
    ULONG StartFrame;
    // number of packets that make up this request
    ULONG NumberOfPackets;
    // number of packets that completed with errors
    ULONG ErrorCount;
    USBD_ISO_PACKET_DESCRIPTOR_64 IsoPacket[1];
};


typedef struct _URB_64 {
    union {
        struct _URB_HEADER_64
            UrbHeader;
        struct _URB_SELECT_INTERFACE_64
            UrbSelectInterface;
        struct _URB_SELECT_CONFIGURATION_64
            UrbSelectConfiguration;
        struct _URB_PIPE_REQUEST_64
            UrbPipeRequest;
        struct _URB_FRAME_LENGTH_CONTROL_64
            UrbFrameLengthControl;
        struct _URB_GET_FRAME_LENGTH_64
            UrbGetFrameLength;
        struct _URB_SET_FRAME_LENGTH_64
            UrbSetFrameLength;
        struct _URB_GET_CURRENT_FRAME_NUMBER_64
            UrbGetCurrentFrameNumber;
        struct _URB_CONTROL_TRANSFER_64
            UrbControlTransfer;
            
        struct _URB_CONTROL_TRANSFER_EX_64
            UrbControlTransferEx;

        struct _URB_PIPE_IO_POLICY_64
            UrbPipeIoPolicy;
    
        struct _URB_BULK_OR_INTERRUPT_TRANSFER_64
            UrbBulkOrInterruptTransfer;
        struct _URB_ISOCH_TRANSFER_64
            UrbIsochronousTransfer;

        // for standard control transfers on the default pipe
        struct _URB_CONTROL_DESCRIPTOR_REQUEST_64
            UrbControlDescriptorRequest;
        struct _URB_CONTROL_GET_STATUS_REQUEST_64
            UrbControlGetStatusRequest;
        struct _URB_CONTROL_FEATURE_REQUEST_64
            UrbControlFeatureRequest;
        struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST_64
            UrbControlVendorClassRequest;
        struct _URB_CONTROL_GET_INTERFACE_REQUEST_64
            UrbControlGetInterfaceRequest;
        struct _URB_CONTROL_GET_CONFIGURATION_REQUEST_64
            UrbControlGetConfigurationRequest;
        struct _URB_OS_FEATURE_DESCRIPTOR_REQUEST_64
            UrbOSFeatureDescriptorRequest;
    };
} URB_64, *PURB_64;

typedef struct _USB_BUS_INTERFACE_USBDI_V0_64 {

    USHORT Size;
    USHORT Version;
    
    ULONG64 BusContext;
    ULONG64 InterfaceReference;
    ULONG64 InterfaceDereference;
    
    // interface specific entries go here

    // the following functions must be callable at high IRQL,
    // (ie higher than DISPATCH_LEVEL)
    
    ULONG64 GetUSBDIVersion;
    ULONG64 QueryBusTime;
    ULONG64 SubmitIsoOutUrb;
    ULONG64 QueryBusInformation;
} USB_BUS_INTERFACE_USBDI_V0_64, *PUSB_BUS_INTERFACE_USBDI_V0_64;

typedef struct _USB_BUS_INTERFACE_USBDI_V1_64 {

    USHORT Size;
    USHORT Version;
    
    ULONG64 BusContext;
    ULONG64 InterfaceReference;
    ULONG64 InterfaceDereference;
    
    // interface specific entries go here

    // the following functions must be callable at high IRQL,
    // (ie higher than DISPATCH_LEVEL)
    
    ULONG64 GetUSBDIVersion;
    ULONG64 QueryBusTime;
    ULONG64 SubmitIsoOutUrb;
    ULONG64 QueryBusInformation;
    ULONG64 IsDeviceHighSpeed;
} USB_BUS_INTERFACE_USBDI_V1_64, *PUSB_BUS_INTERFACE_USBDI_V1_64;

typedef struct _USB_BUS_INTERFACE_USBDI_V2_64 {

    USHORT Size;
    USHORT Version;
    
    ULONG64 BusContext;
    ULONG64 InterfaceReference;
    ULONG64 InterfaceDereference;
    
    // interface specific entries go here

    // the following functions must be callable at high IRQL,
    // (ie higher than DISPATCH_LEVEL)
    
    ULONG64 GetUSBDIVersion;
    ULONG64 QueryBusTime;
    ULONG64 SubmitIsoOutUrb;
    ULONG64 QueryBusInformation;
    ULONG64 IsDeviceHighSpeed;

    ULONG64 EnumLogEntry;
    
} USB_BUS_INTERFACE_USBDI_V2_64, *PUSB_BUS_INTERFACE_USBDI_V2_64;

// version 3 Vista and later

typedef struct _USB_BUS_INTERFACE_USBDI_V3_64 {

    USHORT Size;
    USHORT Version;
    
    ULONG64 BusContext;
    ULONG64 InterfaceReference;
    ULONG64 InterfaceDereference;
    
    // interface specific entries go here

    // the following functions must be callable at high IRQL,
    // (ie higher than DISPATCH_LEVEL)
    
    ULONG64 GetUSBDIVersion;
    ULONG64 QueryBusTime;
    ULONG64 SubmitIsoOutUrb;
    ULONG64 QueryBusInformation;
    ULONG64 IsDeviceHighSpeed;
    ULONG64 EnumLogEntry;

    ULONG64 QueryBusTimeEx;
    ULONG64 QueryControllerType;
           
} USB_BUS_INTERFACE_USBDI_V3_64, *PUSB_BUS_INTERFACE_USBDI_V3_64;

typedef struct _INTERFACE_64 {
    USHORT Size;
    USHORT Version;
    ULONG64 Context;
    ULONG64 InterfaceReference;
    ULONG64 InterfaceDereference;
    // interface specific entries go here
} INTERFACE_64, *PINTERFACE_64;

typedef struct {
    INTERFACE_64 Interface;
    ULONG64   USBCAMD_WaitOnDeviceEvent;
    ULONG64   USBCAMD_BulkReadWrite;
    ULONG64   USBCAMD_SetVideoFormat;
    ULONG64   USBCAMD_SetIsoPipeState;
    ULONG64   USBCAMD_CancelBulkReadWrite;
} USBCAMD_INTERFACE_64, *PUSBCAMD_INTERFACE_64;

};
#endif