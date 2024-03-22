.global sumar
.type sumar, %function

sumar:
    @ Argumentos:
    @   r0 - primer número
    @   r1 - segundo número
    
    ADD r0, r0, r1  @ Suma r0 y r1, resultado en r0
    BX lr           @ Regresa a la función llamadora