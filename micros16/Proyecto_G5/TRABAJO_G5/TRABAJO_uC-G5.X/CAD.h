void	Inic_CAD	(void);
void	Inic_CAD_DMA	(void);
void    inic_OFF_ADC2   (void);
extern unsigned int CAD_BuffA [64] __attribute__((space(dma)));
extern unsigned int CAD2_BuffA [64] __attribute__((space(dma)));