package main

import (
	"net/http"
	"testing"
	"time"
)

// TODO: add unit tests for retry code.

type headerElementTestType = struct {
	in        http.Header
	want      int64
	wantError bool
}

func TestGetTimeoutFromAmcsResponse(t *testing.T) {

	var cases []headerElementTestType

	cases = append(cases, headerElementTestType{http.Header{
		"Request-Context":        {"appId=cid-v1:*******-*********-*********"},
		"Api-Supported-Versions": {"2019-11-02-preview", "2020-04-01-preview", "2020-08-01-preview"},
		"Date":                   {"Thu, 06 May 2021 21:17:42 GMT"},
		"Cache-Control":          {"public", "max-age=3600"},
		"Content-Type":           {"application/json; charset=utf-8"},
		"Vary":                   {"Accept-Encoding"},
		"Server":                 {"Microsoft-HTTPAPI/2.0"},
	}, 3600, false})

	cases = append(cases, headerElementTestType{http.Header{
		"Request-Context":        {"appId=cid-v1:*******-*********-*********"},
		"Api-Supported-Versions": {"2019-11-02-preview", "2020-04-01-preview", "2020-08-01-preview"},
		"Date":                   {"Thu, 06 May 2021 21:17:42 GMT"},
		"Cache-Control":          {"public", "max-age=300"},
		"Content-Type":           {"application/json; charset=utf-8"},
		"Vary":                   {"Accept-Encoding"},
		"Server":                 {"Microsoft-HTTPAPI/2.0"},
	}, 300, false})

	cases = append(cases, headerElementTestType{http.Header{
		"Request-Context": {"appId=cid-v1:*******-*********-*********"},
	}, 0, true})

	for _, c := range cases {
		got, err := getExpirationFromAmcsResponse(c.in)
		if (err != nil) != c.wantError {
			t.Errorf("getTimeoutFromAmcsResponse() did not return correct error, expected: %v, got %s", c.wantError, err)
		} else if err == nil && (got < c.want+time.Now().Unix()-1 || got > c.want+time.Now().Unix()) {
			// Adding a range of 1 second should be enough to keep this test from being flaky, but it might have to be re-thought.
			t.Errorf("getTimeoutFromAmcsResponse() (%v) == %d, want %d", c.in, got, c.want+time.Now().Unix())
		}
	}
}
