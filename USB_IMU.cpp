#include "USB_IMU.h"

USB_COM_IMU::USB_COM_IMU(int in_length_8b, int out_length_8b, uint16_t _vendor_id, uint16_t _product_id,
                         unsigned char _endpoint_in, unsigned char _endpoit_out):
                         N_Communication::USB_COM(in_length_8b,out_length_8b,_vendor_id,_product_id),
                         endpoint_in(_endpoint_in), endpoint_out(_endpoit_out){
    usb_in_data = new usb_imu_rx_cmd_t();
    usb_data_drv = new usb_imu_rx_data_t();
}

USB_COM_IMU::~USB_COM_IMU() {
    delete usb_in_data;
    delete usb_data_drv;
}

void USB_COM_IMU::USB_Com_Start_Trans_Asy(void (*cbf_wrapper)(struct libusb_transfer *)) {
    libusb_fill_bulk_transfer(transfer_rx, get_device_handle(), endpoint_in, rx_buff, usb_message_in_length
    , cbf_wrapper,this,0);
    libusb_submit_transfer(transfer_rx);
    if(transfer_rx->status == 0)
    {
        std::cout << "[USB IMU Asynchronously Receiving]\n";
    }
    else
    {
        std::cout << "[Error] Can not start transmitting\n";
        std::abort();
    }
}

void USB_COM_IMU::Deal_Out_Data() {

}

void USB_COM_IMU::Deal_In_Data() {
    for(int i = 0; i < usb_message_8_2_32; i ++)
    {
        ((uint32_t*)usb_in_data)[i] = (rx_buff[4 * i + 3] << 24) + (rx_buff[4 * i + 2] << 16) + (rx_buff[4 * i + 1] << 8) + rx_buff[4 * i];
    }

    auto* temp_cmd = (uint32_t*)usb_in_data;
    uint32_t t = 0;
    for (size_t i = 0; i < usb_message_in_checklength; i++)
        t = t ^ temp_cmd[i];
    if(usb_in_data->checksum == t)
    {
        memcpy(usb_data_drv, usb_in_data, usb_message_in_length - 4);
    } else {
        std::cout << "[ERROR] USB RX CMD CHECKSUM ERROR!\n";
    }
}

void USB_COM_IMU::USB_In_CBF(struct libusb_transfer *transfer) {
    if(transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        std::cout << "[ERROR] Asy Trans Failed! Try again!\n";
        libusb_submit_transfer(transfer);
    }
    else if(transfer->status == LIBUSB_TRANSFER_COMPLETED)
    {
        lock_mutex_rx();
        this->Deal_In_Data();
        unlock_mutex_rx();
        if(if_print)
            this->Print_Rx_Data();
        libusb_submit_transfer(transfer);
    }
}

void USB_COM_IMU::USB_Out_CBF(struct libusb_transfer *transfer) {

}

void USB_COM_IMU::Print_Rx_Data() {
    std::cout.precision(3);
    std::cout << "[IMU DATA] [" << usb_data_drv->q[0] << " " << usb_data_drv->q[1] << " " <<
    usb_data_drv->q[2] << " " << usb_data_drv->q[3] << "] [" << usb_data_drv->gyro[0] << " " <<
    usb_data_drv->gyro[1] << " " << usb_data_drv->gyro[2] << "] [" << usb_data_drv->accel[0] << " " <<
    usb_data_drv->accel[1] << " " << usb_data_drv->accel[2] << "]" << std::endl;
}
