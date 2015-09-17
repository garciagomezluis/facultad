aproximarPi <- function(cantPuntos = 1000){
	x <- runif(cantPuntos, 0, 1)
	h <- function(x){
		return(sqrt(1 - x^2))
	}
	promedio <- mean(h(x))
	return(4 * promedio)
}
