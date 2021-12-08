FROM golang:1.14.1-alpine
RUN mkdir /app
ADD . /app
WORKDIR /app
RUN go mod download
RUN go build -o apiproxy .
CMD ["/app/apiproxy"]