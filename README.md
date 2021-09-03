# How to

1. Plug in your device
2. Install [drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) 
3. Run
    ```shell
    pio init --ide=vscode
    pio run
    pio run -t upload
    pio device monitor
    ```
4. Enjoy the glory