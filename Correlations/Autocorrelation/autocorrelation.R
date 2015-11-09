autocorrelation <- function(data){
  N = ncol(data)/3
  data1 <- data[seq(3,N*3,3)]
  
  data2 <- matrix(nrow = nrow(data1),ncol = 2*N)
  
  for(i in 1:N){
    data2[,(2*i-1)] <- cos(data1[,i])
    data2[,2*i] <- sin(data1[,i])
  }
  
  vx = mean(data2[,seq(1,2*N-1,2)])
  vy = mean(data2[,seq(2,2*N,2)])
  
  for(i in 1:N){
    data2[,(2*i-1)] <- data2[,(2*i-1)] - vx
    data2[,2*i] <- data2[,2*i] - vy
  }
  
  T = nrow(data2)
  
  
  
  data3 <- matrix(nrow = T-1,ncol = N)
  for(i in 1:N){
    total <- 0
    for(tau in 0:(T-10)){
    
      for(t in 1:(T-tau)){
        
       total = total + sum(data2[t,(2*i-1):(2*i)]*data2[(t+tau),(2*i-1):(2*i)])
     }
    
      total <- total/(T-tau+1)
    
      data3[tau,i] <- total
     }
  }
  
  time <- 1:(T-1)
  autocorrelation <- rowMeans(data3)
  autocorrelation <- autocorrelation/autocorrelation[1]
  plot(time,autocorrelation)
}