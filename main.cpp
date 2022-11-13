#include <iostream>
#include "USB_IMU.h"
#include <memory>

const uint16_t vendor_id1 = 0x1111;
const uint16_t product_id1 = 0x2222;
const unsigned char endpoint_1 = 0x81;
const int IMU_RX_WORDS_PER_MESSAGE = 44;
void imu_cbf_wrapper(struct libusb_transfer* _transfer);

int main() {
    //rx only: tx bits 4*n
    std::shared_ptr<USB_COM_IMU> my_imu(new USB_COM_IMU(IMU_RX_WORDS_PER_MESSAGE,
                                                        4, vendor_id1, product_id1,
                                                        endpoint_1, 0x00));
    my_imu->USB_Com_Start_Trans_Asy(imu_cbf_wrapper);
    while(true)
    {
        libusb_handle_events(my_imu->get_usb_ctx());
    }
}

void imu_cbf_wrapper(struct libusb_transfer* _transfer)
{
    auto* temp = reinterpret_cast<USB_COM_IMU*>(_transfer->user_data);
    temp->USB_In_CBF(_transfer);
}
