// https://github.com/achievements-app/psn-api
// npm install psn-api
import fs from "fs";

import type { Trophy } from "psn-api";
import {
  exchangeCodeForAccessToken,
  exchangeNpssoForCode,
  getTitleTrophies,
  getUserTitles,
  getUserTrophiesEarnedForTitle,
  makeUniversalSearch,
  TrophyRarity
} from "psn-api";

async function main() {
  // 1. Authenticate and become authorized with PSN.
  // See the Authenticating Manually docs for how to get your NPSSO.
  // https://ca.account.sony.com/api/v1/ssocookie
  const npsso = "pnolqZZlow8QqgzVe6kOMVARrPKoVDNxUp26QyxH2wY36mWKYWgonikZGsLl8eXl";
  const accessCode = await exchangeNpssoForCode(npsso);
  const authorization = await exchangeCodeForAccessToken(accessCode);

  // 2. Get the user's `accountId` from the username.
  const allAccountsSearchResults = await makeUniversalSearch(
    authorization,
    "lipenghua",
    "SocialAllAccounts"
  );

  const targetAccountId =
    allAccountsSearchResults.domainResponses[0].results[0].socialMetadata
      .accountId;
  // console.log(targetAccountId);

  // 3. Get the user's list of titles (games).
  // const { trophyTitles } = await getUserTitles(authorization, targetAccountId);
  const { trophyTitles } = await getUserTitles(authorization, "me");
  // console.log(JSON.stringify(trophyTitles));
  fs.writeFileSync("./all.json", JSON.stringify(trophyTitles, null, 4));

  const games: any[] = [];
  for (const title of trophyTitles) {
    // 4. Get the list of trophies for each of the user's titles.
    const { trophies: titleTrophies } = await getTitleTrophies(
      authorization,
      title.npCommunicationId,
      "all",
      {
        npServiceName:
          title.trophyTitlePlatform !== "PS5" ? "trophy" : undefined
      }
    );

    // 5. Get the list of _earned_ trophies for each of the user's titles.
    const { trophies: earnedTrophies } = await getUserTrophiesEarnedForTitle(
      authorization,
      // targetAccountId,
      "me",
      title.npCommunicationId,
      "all",
      {
        npServiceName:
          title.trophyTitlePlatform !== "PS5" ? "trophy" : undefined
      }
    );

    // 6. Merge the two trophy lists.
    const mergedTrophies = mergeTrophyLists(titleTrophies, earnedTrophies);

    games.push({
      gameName: title.trophyTitleName,
      platform: title.trophyTitlePlatform,
      trophyTypeCounts: title.definedTrophies,
      earnedCounts: title.earnedTrophies,
      trophyList: mergedTrophies
    });
  }

  // 7. Write to a JSON file.
  fs.writeFileSync("./games.json", JSON.stringify(games, null, 4));
}

const mergeTrophyLists = (
  titleTrophies: Trophy[],
  earnedTrophies: Trophy[]
) => {
  const mergedTrophies: any[] = [];

  for (const earnedTrophy of earnedTrophies) {
    const foundTitleTrophy = titleTrophies.find(
      (t) => t.trophyId === earnedTrophy.trophyId
    );

    mergedTrophies.push(
      normalizeTrophy({ ...earnedTrophy, ...foundTitleTrophy })
    );
  }

  return mergedTrophies;
};

const normalizeTrophy = (trophy: Trophy) => {
  return {
    isEarned: trophy.earned ?? false,
    earnedOn: trophy.earned ? trophy.earnedDateTime : "unearned",
    type: trophy.trophyType,
    rarity: rarityMap[trophy.trophyRare ?? 0],
    earnedRate: Number(trophy.trophyEarnedRate),
    trophyName: trophy.trophyName,
    groupId: trophy.trophyGroupId
  };
};

const rarityMap: Record<TrophyRarity, string> = {
  [TrophyRarity.VeryRare]: "Very Rare",
  [TrophyRarity.UltraRare]: "Ultra Rare",
  [TrophyRarity.Rare]: "Rare",
  [TrophyRarity.Common]: "Common"
};

main();
