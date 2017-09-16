#ifndef _I2CBUS_H_
#define _I2CBUS_H_

#include "driver/i2c.h"
#include "esp_err.h"

// Config
#define TIMEOUT_DEFAULT     (1000)      /*!< Timeout in milliseconds */
#define I2C_ESP_LOG         ESP_LOGV    /*!< Comment out or change log level, logging stuff will be the same at all levels */

// Receive Acknowledge
#define ACK_CHECK_ENABLE    (0x1)       /*!< Every write is checked by default*/
#define ACK_CHECK_DISABLE   (0x0)
// Send Acknowledge
#define ACK_LEVEL           (0x0)       /*!< Burst readings are ACK*/
#define NACK_LEVEL          (0x1)       /*!< Last reading is NACK*/



class i2cbus_t {
private:
    i2c_port_t port;            /*!< I2C port: I2C_NUM_0 or I2C_NUM_1 */
    uint32_t ticks_to_wait;     /*!< Timeout in ticks for read and write */

public:
    i2cbus_t(i2c_port_t _port) : port(_port), ticks_to_wait(TIMEOUT_DEFAULT / portTICK_PERIOD_MS) {}

    /** *** I2C Begin ***
     * @brief  Config I2C bus and Install Driver
     * @param  sda_io_num    [GPIO number for SDA line]
     * @param  scl_io_num    [GPIO number for SCL line]
     * @param  sda_pullup_en [Enable internal pullup on SDA line]
     * @param  scl_pullup_en [Enable internal pullup on SCL line]
     * @param  clk_speed     [I2C clock frequency for master mode, (no higher than 1MHz for now), Default 100KHz]
     *                       @see "driver/i2c.h"
     * @return               - ESP_OK   Success
     *                       - ESP_ERR_INVALID_ARG Parameter error
     *                       - ESP_FAIL Driver install error
     */
    esp_err_t begin(gpio_num_t sda_io_num, gpio_num_t scl_io_num, gpio_pullup_t sda_pullup_en, gpio_pullup_t scl_pullup_en, uint32_t clk_speed = 100000L);
    esp_err_t begin(gpio_num_t sda_io_num, gpio_num_t scl_io_num, uint32_t clk_speed = 100000L);

    /**
     * Stop I2C bus and unninstall driver
     */
    esp_err_t close();

    /**
     * Timeout read and write
     */
    void set_timeout(uint32_t ms);


    /**
     * *** WRITING interface ***
     * @brief  I2C commands for writing to a 8-bit slave device register.
     *         All of them returns standard esp_err_t codes. So it can be used
     *         within ESP_ERROR_CHECK();
     * @param  address   [I2C slave device register]
     * @param  _register [Register address to write to]
     * @param  bit       [Number of the bit position to write (bit 7~0)]
     * @param  data      [Value(s) to be write to the register]
     * @param  length    [Number of bytes to write (should be within the data buffer size)]
     * @return  - ESP_OK Success
     *          - ESP_ERR_INVALID_ARG Parameter error
     *          - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
     *          - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
     *          - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.
     */
    esp_err_t write_bit(uint8_t address, uint8_t _register, uint8_t bit, uint8_t data);
    esp_err_t write_bits(uint8_t address, uint8_t _register, uint8_t bitstart, uint8_t length, uint8_t data);
    esp_err_t write_byte(uint8_t address, uint8_t _register, uint8_t data);
    esp_err_t write_bytes(uint8_t address, uint8_t _register, uint8_t length, uint8_t *data);

    /**
     * *** READING interface ***
     * @breif  I2C commands for reading a 8-bit slave device register.
     *         All of them returns standard esp_err_t codes.So it can be used
     *         within ESP_ERROR_CHECK();
     * @param  address   [I2C slave device register]
     * @param  _register [Register address to read from]
     * @param  bit       [Number of the bit position to write (bit 7~0)]
     * @param  data      [Buffer to store the read data(s)]
     * @param  length    [Number of bytes to read (should be within the data buffer size)]
     * @return  - ESP_OK Success
     *          - ESP_ERR_INVALID_ARG Parameter error
     *          - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
     *          - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
     *          - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.]
     */
    esp_err_t read_bit(uint8_t address, uint8_t _register, uint8_t bit, uint8_t *data);
    esp_err_t read_bits(uint8_t address, uint8_t _register, uint8_t bitstart, uint8_t length, uint8_t *data);
    esp_err_t read_byte(uint8_t address, uint8_t _register, uint8_t *data);
    esp_err_t read_bytes(uint8_t address, uint8_t _register, uint8_t length, uint8_t *data);

    /**
     * @brief  Quick check to see if a slave device responds.
     * @param  address [I2C slave device register]
     * @return  - ESP_OK Success
     *          - ESP_ERR_INVALID_ARG Parameter error
     *          - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
     *          - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
     *          - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.]
     */
    esp_err_t test(uint8_t address);

    /**
     * I2C scanner utility, prints out all device addresses found on I2Cbus.
     */
    void scanner();
};


// OBJECTS
extern i2cbus_t i2c0;
extern i2cbus_t i2c1;






#endif /* end of include guard: _I2CBUS_H_ */
