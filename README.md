# OpenEDM-wire-tensioner

This repository contains the design of the **Wire Tensioning Mechanism**, in particular, hardware design and firmware. Note that this design is a work in progress, it is not complete, and may contain errors

Currently, the electronic hardware is assembled from heavily customized standard modules, as shown in the image below. An integrated hardware design is available in the [Hardware](https://github.com/OpenEDM/OpenEDM-wire-tensioner/tree/main/Hardware) directory, but it is still in the early stages of development

The mechanical design of the wire tensioner is available as a part of the [OpenEDM-wire-machine design](https://github.com/OpenEDM/OpenEDM-wire-machine)

![](https://github.com/OpenEDM/.github/blob/main/images/wire_tensioner.png)

## Main design requirements

The wire tensioning mechanism must fulfill two main tasks:

1. Ensure that the wire in the cutting zone is fed according to the speed setpoint set by the user
2. Ensure that the wire in the cutting zone is tensioned according to the tension setpoint set by the user

## Operation principle

First, this is how the wire runs. On the right side (not shown in the photo) is the wire source; for example, a spool with new wire. On the left side (also not shown in the photo) is the location where the used wire is directed. This could be a spool for collecting the used wire, or a mechanism that cuts the wire into small pieces for convenient disposal:

![](https://github.com/OpenEDM/.github/blob/main/images/wire_path.jpg)

Next, let's take a look at the wire tensioner's main components:

![](https://github.com/OpenEDM/.github/blob/main/images/wire_tensioner_main_components.png/)

The wire enters the system through the **input wire feeder** (1). Then it passes around several passive pulleys. Next, the wire goes through the **wire guides** (6, 5). The **wire contacts** (7, 4), located near the wire guides, supply electricity to the wire. After that, the wire wraps around the last passive pulley, which is a part of the **load cell assembly** (10). Finally, the wire exits the system through the **output wire feeder** (8). All components are mounted on a frame, which consists of an **aluminum part** (2) and a **plastic part** (3). The system is controlled by an **electronic control unit** (9)

The system operates in such a way that the output feeder pulls the wire, while the input feeder, when necessary, slows the wire's movement by acting as a brake. The operation of the motors depends on the setpoints for speed and tension, as well as on the feedback signal from the load cell. More details are provided below

There is an important design constraint for the input feeder: it must not deform the wire in any way, meaning it must not alter the shape of wire's cross-section. This constraint does not apply to the output feeder

### Input wire feeder

![](https://github.com/OpenEDM/.github/blob/main/images/input_wire_feeder_details.png)

To understand how this feeder works, let's look at how the wire is loaded into it. The wire enters the feeder through a PTFE tube connected to the **input fitting** (1.2). About one meter of wire should be pulled out from the tube, then placed into the groove of the **input latch** (1.1), and the latch should be closed. Next, the wire should be wrapped 4–5 times clockwise around the **pulley** (1.3). It doesn't matter if the loops are placed randomly; during operation, the feeder will self-correct their position. After that, without releasing the tension on the wire, the wire should be placed into the groove of the **output latch** (1.4) and the latch should be closed

Some important points:
1. This feeder operates based on the [capstan effect](https://en.wikipedia.org/wiki/Capstan_equation)
1. For the feeder to operate correctly, the motor must rotate only clockwise, and there must always be some tension in the wire after the output latch
1. Regarding the latches: the input and output latches are structurally identical. The input latch is shown in the closed position in the picture above, while the output latch is shown open. A closed latch significantly hinders the wire from passing through but does not block its movement completely

### Output wire feeder

![](https://github.com/OpenEDM/.github/blob/main/images/output_wire_feeder_details.png)

In this feeder, the main components are the pulleys, between which the wire is clamped. **Active puley** (8.6) is mounted directly on the motor shaft. **Passive puley** (8.7) is mounted on the **lever** (8.1) which rotates around the **axis** (8.3). Both pulleys have toothed rims that provide a kinematic connection between them. A **spring** (8.4), which presses against the **stop** (8.5), applies force to the lever, pushing the pulleys together and ensuring that the wire clamped between them cannot slip

Again, let’s look at how the wire is loaded into this feeder. First, the lever (8.1) must be pressed to create a gap between pulleys (8.6) and (8.7). Then, the wire should be inserted into the hole marked with a red arrow. After that, the wire should pass between the pulleys and exit through the **output fitting** (8.2). Finally, the lever should be released so that the wire is clamped between the pulleys

The main feature of this design is the construction of the pulleys. The pulleys must meet the following key requirements:

1. The surface that comes into contact with the wire must be hard enough so that the wire does not leave marks or cause deformation
1. The pulleys must have an integrated toothed rim

If the pulleys were made entirely of metal, manufacturing them would be a complex and costly task. However, a workaround was found that significantly simplifies the situation and reduces the cost. Each pulley is an assembly of three components:

1. An **outer bushing** (8.6.3) and an **inner bushing** (8.6.1). These bushings must be made of steel. However, due to their simple geometry, they can easily be sourced as off-the-shelf parts
1. An **intermediate bushing** (8.6.2) with an integrated toothed rim. This bushing can simply be 3D-printed from a sufficiently strong plastic, such as ABS

All three bushings -- outer, inner, and intermediate -- are press-fitted into one another. Of course, the dimensions of the plastic bushing must be chosen carefully to ensure a tight fit. This solution has been tested and has proven to work reliably in practice

### Load cell assembly

TODO

### Control system operation algorithm

TODO
