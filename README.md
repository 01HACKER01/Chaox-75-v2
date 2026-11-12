# Chaox-75-v2

<img width="911" height="656" alt="image" src="https://github.com/user-attachments/assets/9d36e80b-5870-467b-ba72-b10f4062921c" />
<img width="410" height="329" alt="image" src="https://github.com/user-attachments/assets/0e184ab3-a807-4555-9e2d-b27b0a48235e" />
<img width="1010" height="632" alt="image" src="https://github.com/user-attachments/assets/887b0fd3-700c-49f0-ab09-b6765a803e89" />
<img width="800" height="305" alt="image" src="https://github.com/user-attachments/assets/7e0d5e57-19a0-49bf-9f55-15ddec867746" />
<img width="802" height="293" alt="image" src="https://github.com/user-attachments/assets/051067fd-8080-456e-9e36-00cb07725982" />
<img width="761" height="333" alt="image" src="https://github.com/user-attachments/assets/e66547de-7d9d-4594-8712-abd92a6a7974" />

## what is this ??
this is a keyboard with tri mode connectivity it can be connected with the dongle(2.4gHZ), bluetooth and wired. it also charges while connected to wired 

## why it exists
i have an old evofox keyboard which is not a good keyboard i cant type fast on that keyboard sometimes the keys doesnt get clicked or sometimes teh key gets stuck so i wanted to buy a keyboard but i thought lets make one by myself so thats why i made this.

## how to assemble 
u can just place all the comps on their places on the pcb and solder them and than flash it.

## firmware stuff
this repo has the code and pcb files for my chaox-75 keyboard. i wrote the firmware in c using zephyr rtos for the nrf52840.

### what it does
* 6x15 key matrix
* works over usb (hid)
* custom fn layer with arrow keys, f-key shortcuts, and media controls

### folders
* `keyboard/` - main firmware code
* `hardware/` - pcb and schematic files
* `dongle/` - 2.4ghz dongle code
* `release/` - precompiled firmware files if u dont wanna build it urself

## how to flash
1. plug the keyboard into your pc with a usb-c cable.
2. double press the reset button on the pcb (or short the rst and gnd pins) to put it into bootloader mode. a new drive should pop up on your pc.
3. grab the `.uf2` firmware file from the releases and just drag and drop it onto that drive.
4. it will disconnect and reboot by itself in a couple seconds and ur good to go.

### what u need to build it
* nrf connect sdk 3.4.1
* zephyr rtos
* cmake & west

### status
still a work in progress (pre-release) so expect some bugs. im still working on it and will push updates when i fix things.
## issues 
it has one issue it doesnt have a switch which can change teh connectivity l;ike the dongle bluetooth or the wired thats an issue which will i fix soon.


---

Made for the CHAOX-75 project.

**CHAOX-75**  
*Custom keyboard firmware, built from the ground up.*

## BOM

| Part | Qty | Approx. USD | Link |
| -------------------------------------- | -- | ------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Custom 82-key Keyboard PCB (5 pcs) | 1 | ~$10–25 | [JLCPCB](https://jlcpcb.com/) |
| 1.5mm Anodized Aluminum Switch Plate | 1 | ~$12–22 | [JLCPCB CNC](https://jlcpcb.com/cnc-machining) |
| 3D Printed Keyboard Case (Top & Bottom) | 1 | ~$10–20 | [JLCPCB 3D Print](https://jlcpcb.com/3d-printing) |
| MX mechanical switches (90 pcs) | 1 | ~$18–35 | [Alibaba MX switches](https://www.alibaba.com/trade/search?SearchText=mechanical+keyboard+switches+90pcs) |
| PBT keycap set (82-key / 75%) | 1 | ~$15–30 | [Alibaba PBT keycaps](https://www.alibaba.com/trade/search?SearchText=PBT+keycaps+set) |
| Kailh MX hotswap sockets (100 pcs) | 1 | ~$6–10 | [LCSC Kailh sockets](https://www.lcsc.com/search?q=kailh%20socket) |
| SK6812MINI-E reverse-mount RGB LEDs (100 pcs) | 1 | ~$8–14 | [LCSC SK6812MINI-E](https://www.lcsc.com/search?q=SK6812MINI-E) |
| 1N4148W SOD-123 diodes (100 pcs) | 1 | ~$1–2 | [LCSC 1N4148W](https://www.lcsc.com/search?q=1N4148W) |
| PCB-mount screw-in stabilizers | 1 | ~$6–12 | [Alibaba PCB stabilizers](https://www.alibaba.com/trade/search?SearchText=PCB+mount+stabilizers) |
| EC11 rotary encoder with metal knob | 1 | ~$1–2 | [LCSC EC11 encoder](https://www.lcsc.com/search?q=EC11%20encoder) |
| 0.91" 128×32 I2C OLED display | 1 | ~$1.50–3 | [LCSC 0.91 OLED](https://www.lcsc.com/search?q=0.91%20inch%20OLED%20I2C) |
| 3.7V 2500–3000mAh LiPo battery (JST-PH 2.0) | 1 | ~$5–9 | [Alibaba LiPo battery](https://www.alibaba.com/trade/search?SearchText=3.7V+3000mAh+lipo+battery+JST) |
| Poron plate foam & switch pads | 1 | ~$3–6 | [Alibaba keyboard poron foam](https://www.alibaba.com/trade/search?SearchText=poron+foam+keyboard) |
| M2 heat-set brass inserts (20 pcs) | 1 | ~$1–3 | [Alibaba M2 heat-set inserts](https://www.alibaba.com/trade/search?SearchText=M2+heat+set+inserts) |
| M2 hex socket screws (20 pcs) | 1 | ~$1–2 | [Alibaba M2 screws](https://www.alibaba.com/trade/search?SearchText=M2+hex+screws) |
| Silicone rubber anti-slip feet (4 pcs) | 1 | ~$0.50–1.50 | [Alibaba silicone bumpons](https://www.alibaba.com/trade/search?SearchText=silicone+bumpons) |
| Braided USB-A → USB-C cable | 1 | ~$2–4 | [Alibaba USB-C cable](https://www.alibaba.com/trade/search?SearchText=braided+usb-c+cable) |
| Custom 2.4GHz USB Dongle (PCB + components) | 1 | ~$8–12 | [JLCPCB](https://jlcpcb.com/) |

### 💰 Estimated total

**Parts: About $110–210 USD**

**Estimated shipping: ~$25–45 USD**

**Total including shipping: About $135–255 USD**, depending heavily on the seller, quantity and shipping.

## Made by Chaox And Arnav
## Thank you very much 

