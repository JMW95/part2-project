# Example for the configuration file

# Vendor ID and Device ID
vendor_id  =  0x1172
device_id  =  0x0de4

# related to the pcie and the fixed translation table
pci_dma_bit_range  = 31
tx_base_addr       = 0x80000000
pcie_cra_bar_no    = 0
pcie_cra_base_addr = 0x00000000

# maximum number of DMA controllers is 4
# followings are arrays to define the parameters for the DMA controllers

# type == 0 means no DMA, 1 means DMA Controller, 2 means SG-DMA Controller
dma_type           =          2
dma_irq_no         =          1
dma_ctrl_bar_no    =          0
dma_ctrl_base_addr = 0x00004000

# for the DMA Controller only, you can ignore this if you are using SG-DMA
sdma_data_width    =          0
