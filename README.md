# OpenEDM-wire-tensioner

This repository contains the design of the **Wire tensioning mechanism**, in particular, hardware design and firmware. Note that this design is a work in progress, it is not complete, and may contain errors

Currently, the electronic hardware is assembled from heavily customized standard modules, as shown in the image below. An integrated hardware design is available in the [Hardware](https://github.com/OpenEDM/OpenEDM-wire-tensioner/tree/main/Hardware) directory, but it is still in the early stages of development

The mechanical design of the wire tensioner is available as a part of the [OpenEDM-wire-machine design](https://github.com/OpenEDM/OpenEDM-wire-machine)

![](https://github.com/OpenEDM/.github/blob/main/images/wire_tensioner.png)

## Operation principle

First, this is how the wire runs. On the right side (not shown in the photo) is the wire source; for example, a spool with new wire. On the left side (also not shown in the photo) is the location where the used wire is directed. This could be a spool for collecting the used wire, or a mechanism that cuts the wire into small pieces for convenient disposal:

![](https://github.com/OpenEDM/.github/blob/main/images/wire_path.jpg)

Next, let's take a look at the wire tensioner's main components:

![](https://github.com/OpenEDM/.github/blob/main/images/wire_tensioner_main_components.png/)

The wire enters the system through the **input wire feeder** (1). Then it passes around several passive pulleys. Next, the wire goes through the **wire guides** (6, 5). The **wire contacts** (7, 4), located near the wire guides, supply electricity to the wire. After that, the wire wraps around the last passive pulley, which is a part of the **load cell assembly** (10). Finally, the wire exits the system through the **output wire feeder** (8). All components are mounted on a frame, which consists of an **aluminum part** (2) and a **plastic part** (3). The system is controlled by an **electronic control unit** (9)

### Input wire feeder

![](https://github.com/OpenEDM/.github/blob/main/images/input_wire_feeder_details.png)

To understand how this feeder works, let's look at how the wire is loaded into it. The wire enters the feeder through a PTFE tube connected to the **input fitting** (1.2). About one meter of wire should be pulled out from the tube, then placed into the groove of the **input latch** (1.1), and the latch should be closed. Next, the wire should be wrapped 4–5 times clockwise around the **pulley** (1.3). It doesn't matter if the loops are placed randomly; during operation, the feeder will self-correct their position. After that, without releasing the tension on the wire, the wire should be placed into the groove of the **output latch** (1.4) and the latch should be closed

Some important points:
1. This feeder operates based on the [capstan effect](https://en.wikipedia.org/wiki/Capstan_equation)
2. Regarding the latches: the latches (1.1) and (1.4) are structurally identical. The latch (1.1) is shown in the closed position in the picture above, while latch (1.4) is shown open. A closed latch significantly hinders the wire from passing through but does not block its movement completely
3. For the feeder to operate correctly, the motor must rotate only clockwise, and there must always be some tension in the wire after the output latch (1.4)

### Output wire feeder

TODO

### Load cell assembly

TODO
