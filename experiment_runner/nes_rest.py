import requests
from typing import TypedDict, Literal, Mapping, Any, Optional


class NESRESTApi:
    def __init__(self, host: str, port: int, base_api_url="/v1/nes"):
        self.host = host
        self.port = str(port)
        self.base_api_url = base_api_url
    
    def _get(self, path: str, params: Optional[Mapping[str, Any]] = None):
        url = f"http://{self.host}:{self.port}{self.base_api_url}{path}"
        return requests.get(url, params=params).json()

    def _post(self, path:str, body: Mapping[str,Any]):
        url = f"http://{self.host}:{self.port}{self.base_api_url}{path}"
        headers = {
            "Content-Type": "application/json",
            "cache-control": "no-cache"
        }
        return requests.post(url,headers=headers, data=body).json()

    ConnectivityResponse = TypedDict(
        "Connectivity",
        {
            "statusCode":int,
            "success": bool
        }
    )
    def connectivity(self) -> ConnectivityResponse:
        return self._get("/connectivity/check")
    

    QueryRequest = TypedDict(
        "QueryRequest",
        {
            "userQuery": str,
            "placement": Literal["TopDown", "BottomUp"]
        }
    )
    QueryResponse = TypedDict(
        "QueryResponse",
        {
            "queryId": int
        }
    )
    def submit_query(self, query: QueryRequest) -> QueryResponse:
        return self._post("/query/execute-query", body=query)

    GetQueryStatusResponse = TypedDict(
        "GetQueryStatusResponse",
        {
            "queryId": int,
            "queryString": str,
            "status": str,
            "queryPlan":str,
            "queryMetaData": str
        }
    )
    def get_query_status(self, queryId: int) -> GetQueryStatusResponse:
        return self._get(f"/query/query-status",params={"queryId":str(queryId)})