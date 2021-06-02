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

type headerBeforeAndAfter = struct {
	in   http.Header
	want map[string]bool
}

func TestFormatHeaderForPrinting(t *testing.T) {

	var cases []headerBeforeAndAfter

	cases = append(cases, headerBeforeAndAfter{http.Header{
		"access_token":   {"asdfasdfasdfasdfASDFAJSDKVJASdfjkasldfjasklfdjasDVCKJSAVKLASvlsadvjklasjgdowrhjknvjals;dhflasdfASJDKLcjkalsdASDcjls-asdvalsdvJLKA;VKVCL;MAWRKS;LNFVLXCZNJVKLNASJFDLUASKLHFJA-jkalsdnjcklasdnjkf;asndjlkvnjkasldcjklshefjklzshd"},
		"client_id":      {"39f51a8d-038a-4031-aa7a-696645d1f9cd"},
		"expires_in":     {"85320"},
		"expires_on":     {"1620336793"},
		"ext_expires_in": {"86399"},
		"not_before":     {"1620250093"},
		"resource":       {"https://monitor.azure.com/"},
	}, map[string]bool{
		"access_token: <secret redacted>":                 false,
		"client_id: 39f51a8d-038a-4031-aa7a-696645d1f9cd": false,
		"expires_in: 85320":                               false,
		"expires_on: 1620336793":                          false,
		"ext_expires_in: 86399":                           false,
		"not_before: 1620250093":                          false,
		"resource: https://monitor.azure.com/":            false,
	}})

	for _, c := range cases {
		ch := FormatHeaderForPrinting(&c.in)
		for got := range ch {
			if _, inMap := c.want[got]; !inMap {
				t.Errorf("formatHeaderForPrinting() got \"%s\", unexpected", got)
			}
		}
	}
}
