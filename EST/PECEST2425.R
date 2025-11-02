# PEC 2024-2025
# Alejandro Muñoz Fernández
# Versión: 202504251218

## Código del apartado 1
#
# Número de observaciones; piden 50000
n <- 50000

# Genera n/2 pares de variables uniformes U₁ y U₂, cada una con 25000 variables
set.seed(123)  # Reproduce una secuencia de números aleatorios
U1 <- runif(n / 2)
U2 <- runif(n / 2)

# Aplica la transformación de Box-Müller (conforme a la página 199 del libro)
# Parte de la raíz cuadrada
raiz <- sqrt(-2 * log(U2))
# Parte del ángulo Θ (zeta, teta o theta o como se llame)
angulo <- 2 * pi * U1
X <- raiz * cos(angulo)
Y <- raiz * sin(angulo)

# Combina X e Y para obtener las 50000 observaciones
Z <- c(X, Y)

## Código del apartado 2
#
# Dibuja el histograma de los valores simulados (Z)
hist(
  Z,
  breaks = 50,
  main = "Histograma de frecuencias de Z",
  xlab = "Z",
  ylab = "Frecuencia"
)

## Código del apartado 3
#
# Calcula la P(Z > 1.645)
probabilidad <- sum(Z > 1.645) / length(Z)
# Muestra la P(Z > 1.645)
probabilidad

# Muestra las estadísticas descriptivas de la simulación;
# que no pide el ejercicio, pero me vale para comprobar
# que los datos son coherentes
summary(Z)
# varianza
var(Z)